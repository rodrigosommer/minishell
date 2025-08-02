/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fork_commands(t_command *cmds, t_shell *shell,
			t_pipe_context *ctx)
{
	t_command	*current;
	int			i;

	current = cmds;
	i = 0;
	while (current && i < ctx->cmd_count
		&& current->operator != TOKEN_AND && current->operator != TOKEN_OR)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] == 0)
		{
			setup_pipe_fds(ctx->pipes, i, ctx->cmd_count);
			close_pipes(ctx->pipes, ctx->cmd_count);
			exit(execute_single_command(current, shell));
		}
		i++;
		if (current->operator == TOKEN_PIPE)
			current = current->next;
		else
			break ;
	}
}

static int	count_pipe_commands(t_command *commands)
{
	int			count;
	t_command	*current;

	count = 0;
	current = commands;
	while (current && current->operator != TOKEN_AND
		&& current->operator != TOKEN_OR)
	{
		count++;
		if (current->operator == TOKEN_PIPE)
			current = current->next;
		else
			break ;
	}
	return (count);
}

int	execute_pipeline(t_command *commands, t_shell *shell)
{
	t_pipe_context	ctx;

	if (has_pipe_operators(commands))
	{
		ctx.cmd_count = count_pipe_commands(commands);
		ctx.pipes = create_pipes(ctx.cmd_count);
		if (!ctx.pipes)
			return (1);
		ctx.pids = malloc(sizeof(pid_t) * ctx.cmd_count);
		if (!ctx.pids)
		{
			free(ctx.pipes);
			return (1);
		}
		fork_commands(commands, shell, &ctx);
		close_pipes(ctx.pipes, ctx.cmd_count);
		return (wait_for_children(ctx.pids, ctx.cmd_count, ctx.pipes));
	}
	return (execute_logical_operators(commands, shell));
}

int	execute_logical_operators(t_command *commands, t_shell *shell)
{
	t_command	*current;
	int			status;

	current = commands;
	status = 0;
	while (current)
	{
		status = execute_command(current, shell);
		if (current->operator == TOKEN_AND && status != 0)
		{
			current = current->next;
			while (current && current->operator != TOKEN_OR)
				current = current->next;
		}
		else if (current->operator == TOKEN_OR && status == 0)
		{
			current = current->next;
			while (current && current->operator != TOKEN_AND)
				current = current->next;
		}
		else
			current = current->next;
	}
	return (status);
}
