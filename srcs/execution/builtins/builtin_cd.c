/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:20:03 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:20:04 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*get_cd_path(t_cmd *cmd, t_env *env)
{
	char	*path;

	if (!cmd->args[1] || !ft_strcmp(cmd->args[1], "~"))
	{
		path = get_env_value(env, "HOME");
		if (!path)
		{
			print_error("cd", NULL, "HOME not set");
			return (NULL);
		}
	}
	else if (!ft_strcmp(cmd->args[1], "-"))
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
		{
			print_error("cd", NULL, "OLDPWD not set");
			return (NULL);
		}
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
		path = cmd->args[1];
	return (path);
}

int	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*path;

	path = get_cd_path(cmd, env);
	if (!path)
		return (1);
	oldpwd = getcwd(cwd, PATH_MAX);
	if (chdir(path) == -1)
	{
		print_error("cd", cmd->args[1], strerror(errno));
		return (1);
	}
	set_env_value(&env, "OLDPWD", oldpwd);
	if (!getcwd(cwd, PATH_MAX))
	{
		print_error("cd", "getcwd", strerror(errno));
		return (1);
	}
	set_env_value(&env, "PWD", cwd);
	return (0);
}
