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

    if (pipe(fd) == -1)
        return (print_error("pipe", NULL, strerror(errno)), 1);

    pid = fork();
    if (pid == -1) {
        close(fd[0]);
        close(fd[1]);
        return (print_error("fork", NULL, strerror(errno)), 1);
    }

    if (pid == 0) {
        close(fd[0]);
        if (cmd->out_fd == STDOUT_FILENO)
            cmd->out_fd = fd[1];
        else {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }
        execute_child(cmd, env, data);
    }
    else {
        close(fd[1]);
        if (cmd->next->in_fd == STDIN_FILENO)
            cmd->next->in_fd = fd[0];
        else {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
        }
        execute_command(cmd->next, env, data);
    }
    return 0;
}
