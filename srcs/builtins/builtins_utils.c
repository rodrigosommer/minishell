/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, shell));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, shell));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, shell));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(shell->env));
	if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(args, shell);
	if (ft_strcmp(args[0], "true") == 0)
		return (0);
	if (ft_strcmp(args[0], "false") == 0)
		return (1);
	return (1);
}

static char	**remove_env_var(char **env, char *var_name)
{
	int		j;
	int		k;
	char	**new_env;
	int		env_count;
	int		var_len;

	env_count = count_env_vars(env);
	new_env = malloc(sizeof(char *) * (env_count + 1));
	if (!new_env)
		return (NULL);
	j = 0;
	k = 0;
	var_len = ft_strlen(var_name);
	while (env[j])
	{
		if (ft_strncmp(env[j], var_name, var_len) == 0
			&& (env[j][var_len] == '=' || env[j][var_len] == '\0'))
			free(env[j]);
		else
			new_env[k++] = env[j];
		j++;
	}
	new_env[k] = NULL;
	return (new_env);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int		i;
	char	**new_env;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		new_env = remove_env_var(shell->env, args[i]);
		if (!new_env)
			return (1);
		free(shell->env);
		shell->env = new_env;
		i++;
	}
	return (0);
}

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	if (ft_strcmp(command, "true") == 0)
		return (1);
	if (ft_strcmp(command, "false") == 0)
		return (1);
	return (0);
}
