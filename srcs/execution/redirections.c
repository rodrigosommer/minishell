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

void	handle_redirections(t_cmd *cmd)
{
	if (cmd->in_fd != STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
}

int	handle_heredoc(char *delimiter)
{
	int		fd;
	char	*line;

	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}
