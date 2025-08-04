/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:37:23 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:37:25 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_in_path(char *command, char **env)
{
	char	*path_env;
	char	**paths;
	char	*result;
	int		i;

	path_env = get_env_value("PATH", env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = check_path(command, paths, i);
		if (result)
		{
			free_paths(paths);
			return (result);
		}
		i++;
	}
	free_paths(paths);
	return (NULL);
}

char	*get_command_path(char *command, char **env)
{
	struct stat	st;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == 0)
		{
			if (stat(command, &st) == 0 && S_ISDIR(st.st_mode))
				return (ft_strdup("IS_DIRECTORY"));
			if (access(command, X_OK) == 0)
				return (ft_strdup(command));
			return (ft_strdup("PERMISSION_DENIED"));
		}
		return (ft_strdup("NO_SUCH_FILE"));
	}
	return (find_in_path(command, env));
}

static int	execute_external(char **args, char **env)
{
	char	*path;
	pid_t	pid;
	int		ret;

	path = get_command_path(args[0], env);
	ret = validate_path(path, args[0]);
	if (ret != 0)
	{
		if (path)
			free(path);
		return (ret);
	}
	pid = fork();
	if (pid == 0)
		exec_child(path, args, env);
	else if (pid > 0)
		return (exec_parent(pid, path));
	free(path);
	return (1);
}

int	execute_command(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	if (!cmd || !cmd->args || !cmd->args[0] || !*cmd->args[0])
		return (0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (setup_redirections(cmd->redirects) == -1)
		return (restore_std_fds(saved_stdin, saved_stdout), 1);
	if (is_builtin(cmd->args[0]))
		ret = execute_builtin(cmd->args, shell);
	else
		ret = execute_external(cmd->args, shell->env);
	restore_std_fds(saved_stdin, saved_stdout);
	return (ret);
}
