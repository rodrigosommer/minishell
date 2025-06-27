/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:22:51 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:22:52 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_redirections(t_cmd *cmd)
{
    if (cmd->in_fd != STDIN_FILENO) {
        if (dup2(cmd->in_fd, STDIN_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(cmd->in_fd);
    }
    if (cmd->out_fd != STDOUT_FILENO) {
        if (dup2(cmd->out_fd, STDOUT_FILENO) == -1) {
            print_error("dup2", NULL, strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(cmd->out_fd);
    }
}

int handle_heredoc(char *delimiter)
{
    int fd[2];
    char *line;
    
    if (pipe(fd) == -1)
        return -1;
    
    while (1) {
        line = readline("> ");
        if (!line) {
            // This handles Ctrl+D
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", STDERR_FILENO);
            break;
        }
        if (!ft_strcmp(line, delimiter)) {
            free(line);
            break;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    close(fd[1]);
    return fd[0];
}
