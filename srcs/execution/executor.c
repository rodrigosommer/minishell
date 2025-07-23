/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:22:14 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:22:20 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/executor_utils.h"

static char *find_command_path(char *cmd, t_env *env)
{
    char *full_path;
    char *path_env;
    char **paths;
    int i;
    struct stat statbuf;

    if (!cmd || !*cmd) {
        debug_log("find_command_path", "Empty command", NULL);
        print_error(NULL, NULL, "command not found");
        return NULL;
    }

    debug_log("find_command_path", "Checking command", cmd);
    if (ft_strchr(cmd, '/')) {
        if (stat(cmd, &statbuf) == -1) {
            print_error(cmd, NULL, "No such file or directory");
            debug_log("find_command_path", "File not found", cmd);
            return NULL;
        }
        if (S_ISDIR(statbuf.st_mode)) {
            print_error(cmd, NULL, "Is a directory");
            debug_log("find_command_path", "Is a directory", cmd);
            errno = EISDIR;
            return NULL;
        }
        if (access(cmd, X_OK) == -1) {
            print_error(cmd, NULL, "Permission denied");
            debug_log("find_command_path", "Permission denied", cmd);
            errno = EACCES;
            return NULL;
        }
        return ft_strdup(cmd);
    }

    path_env = get_env_value(env, "PATH");
    if (!path_env) {
        print_error(cmd, NULL, "No such file or directory");
        debug_log("find_command_path", "No PATH variable", NULL);
        return NULL;
    }

    paths = ft_split(path_env, ':');
    if (!paths) {
        debug_log("find_command_path", "Path split failed", NULL);
        return NULL;
    }

    i = -1;
    while (paths[++i]) {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (!full_path) {
            free_array(paths);
            debug_log("find_command_path", "Path join failed", NULL);
            return NULL;
        }
        if (stat(full_path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                print_error(full_path, NULL, "Is a directory");
                debug_log("find_command_path", "Is a directory", full_path);
                free(full_path);
                free_array(paths);
                errno = EISDIR;
                return NULL;
            }
            if (access(full_path, X_OK) == -1) {
                print_error(full_path, NULL, "Permission denied");
                debug_log("find_command_path", "Permission denied", full_path);
                free(full_path);
                free_array(paths);
                errno = EACCES;
                return NULL;
            }
            free_array(paths);
            debug_log("find_command_path", "Found executable", full_path);
            return full_path;
        }
        free(full_path);
    }
    free_array(paths);
    print_error(cmd, NULL, "command not found");
    debug_log("find_command_path", "Command not found", cmd);
    return NULL;
}

void execute_child(t_cmd *cmd, t_env *env, t_data *data)
{
    char *path;
    char **envp;

    debug_log("execute_child", "Executing command", cmd->args ? cmd->args[0] : "NULL");
    
    // Redirections should already be handled by the caller
    // Don't apply them here to avoid double application
    
    debug_log("execute_child", "Redirections handled by caller", NULL);

    // Close extra file descriptors
    for (int fd = 3; fd < 1024; fd++)
        close(fd);

    if (is_builtin(cmd)) {
        // Create a temporary environment pointer for builtins
        t_env *temp_env = env;
        int ret = execute_builtin(cmd, &temp_env, data);
        debug_log("execute_child", "Built-in executed, exit status", ft_itoa(ret));
        exit(ret);
    }

    path = find_command_path(cmd->args[0], env);
    if (!path) {
        int exit_code = 127;
        if (errno == EACCES) exit_code = 126;
        else if (errno == EISDIR) exit_code = 126;
        debug_log("execute_child", "No path found, exiting with", ft_itoa(exit_code));
        exit(exit_code);
    }

    envp = env_to_array(env);
    debug_log("execute_child", "Executing external command", path);
    execve(path, cmd->args, envp);
    
    print_error(path, NULL, strerror(errno));
    debug_log("execute_child", "execve failed", strerror(errno));
    free(path);
    free_array(envp);
    exit(127);
}

int execute_command(t_cmd *cmd, t_env *env, t_data *data)
{
    pid_t pid;
    int status;

    if (!cmd->args || !cmd->args[0]) {
        debug_log("execute_command", "Empty command", NULL);
        return 0;
    }

    debug_log("execute_command", "Executing command", cmd->args[0]);
    g_command_executing = 1;

    // Handle pipes first
    if (cmd->next) {
        int ret = handle_pipes(cmd, env, data);
        g_command_executing = 0;
        debug_log("execute_command", "Pipe command completed, status", ft_itoa(ret));
        return ret;
    }
        
    // Handle single built-in commands
    if (is_builtin(cmd)) {
        // Apply redirections before executing builtin
        int saved_stdout = -1;
        int saved_stdin = -1;
        
        // Save and redirect output
        if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd != -1) {
            saved_stdout = dup(STDOUT_FILENO);
            if (saved_stdout == -1) {
                print_error("dup", NULL, strerror(errno));
                g_command_executing = 0;
                data->exit_status = 1;
                return 1;
            }
            if (dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                close(saved_stdout);
                g_command_executing = 0;
                data->exit_status = 1;
                return 1;
            }
            close(cmd->out_fd);
        }
        
        // Save and redirect input
        if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != -1) {
            saved_stdin = dup(STDIN_FILENO);
            if (saved_stdin == -1) {
                print_error("dup", NULL, strerror(errno));
                if (saved_stdout != -1) {
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdout);
                }
                g_command_executing = 0;
                data->exit_status = 1;
                return 1;
            }
            if (dup2(cmd->in_fd, STDIN_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                close(saved_stdin);
                if (saved_stdout != -1) {
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdout);
                }
                g_command_executing = 0;
                data->exit_status = 1;
                return 1;
            }
            close(cmd->in_fd);
        }

        int ret = execute_builtin(cmd, &env, data);
        
        // Restore original file descriptors
        if (saved_stdout != -1) {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        if (saved_stdin != -1) {
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
        
        g_command_executing = 0;
        data->exit_status = ret;
        debug_log("execute_command", "Built-in completed, status", ft_itoa(ret));
        return ret;
    }

    // Handle single external commands
    pid = fork();
    if (pid == -1) {
        g_command_executing = 0;
        print_error("fork", NULL, strerror(errno));
        debug_log("execute_command", "Fork failed", NULL);
        data->exit_status = 1;
        return 1;
    }

    if (pid == 0) {
        setup_child_signals();
        
        // Apply redirections in child process for single commands
        if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != -1) {
            if (dup2(cmd->in_fd, STDIN_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(cmd->in_fd);
        }
        
        if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd != -1) {
            if (dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(cmd->out_fd);
        }
        
        execute_child(cmd, env, data);
    }
    else {
        // Parent process - close file descriptors if they were opened
        if (cmd->in_fd != STDIN_FILENO && cmd->in_fd != -1)
            close(cmd->in_fd);
        if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd != -1)
            close(cmd->out_fd);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            data->exit_status = WEXITSTATUS(status);
            debug_log("execute_command", "Child exited with status", ft_itoa(data->exit_status));
        }
        else if (WIFSIGNALED(status)) {
            if (WTERMSIG(status) == SIGINT) {
                write(STDOUT_FILENO, "\n", 1);
                data->exit_status = 128 + SIGINT;
            }
            else if (WTERMSIG(status) == SIGQUIT) {
                ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
                data->exit_status = 128 + SIGQUIT;
            } else {
                data->exit_status = 128 + WTERMSIG(status);
            }
            debug_log("execute_command", "Child signaled, status", ft_itoa(data->exit_status));
        }
        g_command_executing = 0;
    }
    return data->exit_status;
}

int is_builtin(t_cmd *cmd)
{
    if (!cmd->args[0])
        return (0);
    if (!ft_strcmp(cmd->args[0], "echo"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "cd"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "pwd"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "export"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "unset"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "env"))
        return (1);
    if (!ft_strcmp(cmd->args[0], "exit"))
        return (1);
    return (0);
}

int execute_builtin(t_cmd *cmd, t_env **env, t_data *data)
{
    int ret = 0;

    // Execute the builtin command (no redirection handling here since it's done in execute_command)
    if (!ft_strcmp(cmd->args[0], "exit")) {
        ret = builtin_exit(cmd, data);
        if (ret == 1) {
            data->exit_status = 1;
        }
    }
    else if (!ft_strcmp(cmd->args[0], "echo"))
        ret = builtin_echo(cmd);
    else if (!ft_strcmp(cmd->args[0], "cd"))
        ret = builtin_cd(cmd, *env);
    else if (!ft_strcmp(cmd->args[0], "pwd"))
        ret = builtin_pwd();
    else if (!ft_strcmp(cmd->args[0], "export"))
        ret = builtin_export(cmd, env);
    else if (!ft_strcmp(cmd->args[0], "unset"))
        ret = builtin_unset(cmd, env);
    else if (!ft_strcmp(cmd->args[0], "env"))
        ret = builtin_env(*env);

    return ret;
}

char **env_to_array(t_env *env)
{
    t_env   *tmp;
    char    **envp;
    int     i;
    int     count;

    count = 0;
    tmp = env;
    while (tmp)
    {
        if (tmp->value)
            count++;
        tmp = tmp->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    i = 0;
    tmp = env;
    while (tmp && i < count)
    {
        if (tmp->value)
        {
            envp[i] = ft_strjoin3(tmp->key, "=", tmp->value);
            if (!envp[i])
                return (free_array(envp), NULL);
            i++;
        }
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
}

