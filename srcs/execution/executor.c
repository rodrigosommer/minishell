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

    if (access(cmd, X_OK) == 0)
        return ft_strdup(cmd);

    path_env = get_env_value(env, "PATH");
    if (!path_env)
        return NULL;

    paths = ft_split(path_env, ':');
    if (!paths)
        return NULL;

    i = -1;
    while (paths[++i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_array(paths);
            return full_path;
        }
        free(full_path);
    }
    free_array(paths);
    return NULL;
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
    int saved_stdout = -1;
    int saved_stdin = -1;
    int ret = 0;

    // Apply output redirection first
    if (cmd->out_fd != STDOUT_FILENO) {
        saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdout == -1 || dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
            print_error("dup/dup2", NULL, strerror(errno));
            if (saved_stdout != -1) close(saved_stdout);
            return 1;
        }
        close(cmd->out_fd);  // Close the original fd after duplication
    }

    // Then apply input redirection
    if (cmd->in_fd != STDIN_FILENO) {
        saved_stdin = dup(STDIN_FILENO);
        if (saved_stdin == -1 || dup2(cmd->in_fd, STDIN_FILENO) == -1) {
            print_error("dup/dup2", NULL, strerror(errno));
            // Clean up output redirection if it was set
            if (saved_stdout != -1) {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
            return 1;
        }
        close(cmd->in_fd);  // Close the original fd after duplication
    }

    // Execute the builtin command
    if (!ft_strcmp(cmd->args[0], "echo"))
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
    else if (!ft_strcmp(cmd->args[0], "exit"))
        builtin_exit(cmd, data);

    // Restore original file descriptors
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }

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

void execute_child(t_cmd *cmd, t_env *env, t_data *data)
{
    // Apply redirections
    if (cmd->in_fd != STDIN_FILENO) {
        dup2(cmd->in_fd, STDIN_FILENO);
        close(cmd->in_fd);
    }
    if (cmd->out_fd != STDOUT_FILENO) {
        dup2(cmd->out_fd, STDOUT_FILENO);
        close(cmd->out_fd);
    }

    if (is_builtin(cmd))
        exit(execute_builtin(cmd, &env, data));

    char *path = find_command_path(cmd->args[0], env);
    if (!path) {
        print_error(cmd->args[0], NULL, "command not found");
        exit(127);
    }

    char **envp = env_to_array(env);
    execve(path, cmd->args, envp);
    print_error(path, NULL, strerror(errno));
    free(path);
    free_array(envp);
    exit(126);
}

int execute_command(t_cmd *cmd, t_env *env, t_data *data)
{
    pid_t pid;
    int status;

    if (!cmd->args[0])
        return 0;

    if (is_builtin(cmd) && !cmd->next)
        return execute_builtin(cmd, &env, data);

    pid = fork();
    if (pid == -1)
        return (print_error("fork", NULL, strerror(errno)), 1);

    if (pid == 0) {
        // Apply input redirection first
        if (cmd->in_fd != STDIN_FILENO) {
            if (dup2(cmd->in_fd, STDIN_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(cmd->in_fd);
        }

        // Then apply output redirection
        if (cmd->out_fd != STDOUT_FILENO) {
            if (dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
                print_error("dup2", NULL, strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(cmd->out_fd);
        }

        if (is_builtin(cmd))
            exit(execute_builtin(cmd, &env, data));

        char *path = find_command_path(cmd->args[0], env);
        if (!path) {
            print_error(cmd->args[0], NULL, "command not found");
            exit(127);
        }

        char **envp = env_to_array(env);
        execve(path, cmd->args, envp);
        print_error(path, NULL, strerror(errno));
        free(path);
        free_array(envp);
        exit(126);
    }
    else {
        // Close file descriptors in parent
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
    }
    return 0;
}
