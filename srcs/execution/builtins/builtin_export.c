/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:25:02 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:25:03 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	print_export(t_env *env)
{
	while (env)
	{
		ft_printf("declare -x %s", env->key);
		if (env->value)
			ft_printf("=\"%s\"", env->value);
		ft_printf("\n");
		env = env->next;
	}
	return (0);
}

static int	add_env_var(t_env **env, char *arg)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(arg, '=');
	if (!equal)
		key = ft_strdup(arg);
	else
		key = ft_substr(arg, 0, equal - arg);
	if (!key)
		return (1);
	if (equal)
		value = ft_strdup(equal + 1);
	else
		value = NULL;
	set_env_value(env, key, value);
	free(key);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_env **env)
{
	int	i;
	int	ret;

	if (!cmd->args[1])
		return (print_export(*env));
	ret = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_env_key(cmd->args[i]))
		{
			print_error("export", cmd->args[i], "not a valid identifier");
			ret = 1;
		}
		else if (add_env_var(env, cmd->args[i]))
			return (1);
		i++;
	}
	return (ret);
}
