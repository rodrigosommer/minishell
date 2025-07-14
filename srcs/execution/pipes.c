/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:23:12 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:23:17 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Forward declarations for static functions from executor.c
int is_builtin(t_cmd *cmd);
int execute_builtin(t_cmd *cmd, t_env **env, t_data *data);
char **env_to_array(t_env *env);

int handle_pipes(t_cmd *cmd, t_env *env, t_data *data)
{
    int fd[2];
    pid_t pid;
    int status;
    int ret = 0;

    debug_log("handle_pipes", "Creating pipe for command", cmd->args ? cmd->args[0] : "NULL");
    if (pipe(fd) == -1) {
        print_error("pipe", NULL, strerror(errno));
        debug_log("handle_pipes", "Pipe creation failed", strerror(errno));
        data->exit_status = 1;
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        close(fd[0]);
        close(fd[1]);
        print_error("fork", NULL, strerror(errno));
        debug_log("handle_pipes", "Fork failed", NULL);
        data->exit_status = 1;
        return 1;
    }

    if (pid == 0) {  // Child process (left side of pipe)
        debug_log("handle_pipes", "In child process", cmd->args ? cmd->args[0] : "NULL");
        close(fd[0]);
        setup_child_signals();
        
        handle_redirections(cmd);
        debug_log("handle_pipes", "Applied redirections in child", NULL);
        
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            debug_log("handle_pipes", "dup2 failed in child", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
        
        execute_child(cmd, env, data);
        exit(EXIT_SUCCESS);
    }
    else {  // Parent process (right side of pipe)
        debug_log("handle_pipes", "In parent process", cmd->next->args ? cmd->next->args[0] : "NULL");
        close(fd[1]);
        
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            close(fd[0]);
            print_error("dup2", NULL, strerror(errno));
            debug_log("handle_pipes", "dup2 failed in parent", strerror(errno));
            data->exit_status = 1;
            return 1;
        }
        close(fd[0]);
        
        handle_redirections(cmd->next);
        debug_log("handle_pipes", "Applied redirections in parent", NULL);
        
        ret = execute_command(cmd->next, env, data);
        debug_log("handle_pipes", "Right side exit status", ft_itoa(ret));
        
        dup2(data->original_stdin, STDIN_FILENO);
        
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            data->exit_status = WEXITSTATUS(status);
            debug_log("handle_pipes", "Child exit status", ft_itoa(data->exit_status));
        }
        else if (WIFSIGNALED(status)) {
            data->exit_status = 128 + WTERMSIG(status);
            debug_log("handle_pipes", "Child signaled", ft_itoa(data->exit_status));
        }
        return ret;
    }
}
