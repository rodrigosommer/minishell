/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:37:42 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:37:44 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_command *commands)
{
	int	count;

	count = 0;
	while (commands)
	{
		count++;
		commands = commands->next;
	}
	return (count);
}

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (setup_redirections(cmd->redirects) == -1)
		return (1);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd->args, shell));
	return (execute_external_no_wait(cmd->args, shell->env));
}

int	wait_for_children(pid_t *pids, int cmd_count, int **pipes)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			last_status = WEXITSTATUS(status);
		i++;
	}
	i = 0;
	while (i < cmd_count - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
	return (last_status);
}

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		i++;
	while (--i >= 0)
	{
		free(paths[i]);
	}
	free(paths);
}
