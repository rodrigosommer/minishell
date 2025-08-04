/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:37:13 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:37:16 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_path(char *path, char *command)
{
	if (!path)
	{
		dprintf(2, "minishell: %s: command not found\n", command);
		return (127);
	}
	if (ft_strcmp(path, "NO_SUCH_FILE") == 0)
	{
		dprintf(2, "minishell: %s: No such file or directory\n", command);
		return (127);
	}
	if (ft_strcmp(path, "PERMISSION_DENIED") == 0)
	{
		dprintf(2, "minishell: %s: Permission denied\n", command);
		return (126);
	}
	if (ft_strcmp(path, "IS_DIRECTORY") == 0)
	{
		dprintf(2, "minishell: %s: Is a directory\n", command);
		return (126);
	}
	return (0);
}

void	exec_child(char *path, char **args, char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, args, env);
	perror("execve");
	exit(127);
}

int	exec_parent(pid_t pid, char *path)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	handle_signals();
	free(path);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_printf("\n");
		else if (WTERMSIG(status) == SIGQUIT)
			ft_printf("Quit: 3\n");
		return (128 + WTERMSIG(status));
	}
	return (WEXITSTATUS(status));
}

char	*check_path(char *command, char **paths, int i)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(paths[i], "/");
	full_path = ft_strjoin(temp, command);
	free(temp);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

int	execute_external_no_wait(char **args, char **env)
{
	char	*path;

	path = get_command_path(args[0], env);
	if (!path)
	{
		dprintf(2, "minishell: %s: command not found\n", args[0]);
		return (127);
	}
	if (ft_strcmp(path, "PERMISSION_DENIED") == 0)
	{
		dprintf(2, "minishell: %s: Permission denied\n", args[0]);
		free(path);
		return (126);
	}
	if (ft_strcmp(path, "IS_DIRECTORY") == 0)
	{
		dprintf(2, "minishell: %s: Is a directory\n", args[0]);
		free(path);
		return (126);
	}
	execve(path, args, env);
	perror("execve");
	free(path);
	return (127);
}
