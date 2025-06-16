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

int	builtin_cd(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];

	if (!cmd->args[1] || !ft_strcmp(cmd->args[1], "~"))
	{
		path = get_env_value(env, "HOME");
		if (!path)
			return (print_error("cd", NULL, "HOME not set"), 1);
	}
	else if (!ft_strcmp(cmd->args[1], "-"))
	{
		path = get_env_value(env, "OLDPWD");
		if (!path)
			return (print_error("cd", NULL, "OLDPWD not set"), 1);
		ft_putendl_fd(path, STDOUT_FILENO);
	}
	else
		path = cmd->args[1];
	oldpwd = getcwd(cwd, PATH_MAX);
	if (chdir(path) == -1)
		return (print_error("cd", cmd->args[1], strerror(errno)), 1);
	set_env_value(&env, "OLDPWD", oldpwd);
	if (!getcwd(cwd, PATH_MAX))
		return (print_error("cd", "getcwd", strerror(errno)), 1);
	set_env_value(&env, "PWD", cwd);
	return (0);
}
