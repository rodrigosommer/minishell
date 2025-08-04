/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:36:33 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:36:36 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*home;

	if (!args[1])
	{
		home = get_env_value("HOME", shell->env);
		if (!home)
		{
			dprintf(2, "minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (home);
	}
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;

	if (args[1] && args[2])
	{
		dprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	path = get_cd_path(args, shell);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (builtin_env(shell->env));
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				args[i]);
			return (1);
		}
		if (ft_strchr(args[i], '='))
			shell->env = add_env_var(shell->env, args[i]);
		i++;
	}
	return (0);
}
