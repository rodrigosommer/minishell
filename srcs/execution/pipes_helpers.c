/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:37:33 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:37:35 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	**create_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	if (cmd_count <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	setup_pipe_fds(int **pipes, int cmd_idx, int cmd_count)
{
	if (cmd_idx > 0)
	{
		dup2(pipes[cmd_idx - 1][0], STDIN_FILENO);
	}
	if (cmd_idx < cmd_count - 1)
	{
		dup2(pipes[cmd_idx][1], STDOUT_FILENO);
	}
}

int	has_pipe_operators(t_command *commands)
{
	t_command	*current;

	current = commands;
	while (current)
	{
		if (current->operator == TOKEN_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}
