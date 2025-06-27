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

    if (pipe(fd) == -1) {
        print_error("pipe", NULL, strerror(errno));
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        close(fd[0]);
        close(fd[1]);
        print_error("fork", NULL, strerror(errno));
        return 1;
    }

    if (pid == 0) {  // Child process (left side of pipe)
        close(fd[0]);  // Close read end
        setup_child_signals();  // Ensure proper signal handling in child
        
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd[1]);
        
        execute_child(cmd, env, data);
        exit(EXIT_SUCCESS);
    }
    else {  // Parent process (right side of pipe)
        close(fd[1]);  // Close write end
        
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            close(fd[0]);
            print_error("dup2", NULL, strerror(errno));
            return 1;
        }
        close(fd[0]);
        
        // Execute the next command in the pipe
        int ret = execute_command(cmd->next, env, data);
        
        // Restore stdin
        dup2(data->original_stdin, STDIN_FILENO);
        
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            data->exit_status = WEXITSTATUS(status);
        }
        return ret;
    }
}
