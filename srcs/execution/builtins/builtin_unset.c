/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:25:27 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:25:28 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	while (curr && ft_strcmp(curr->key, key))
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr)
	{
		if (prev)
			prev->next = curr->next;
		else
			*env = curr->next;
		free(curr->key);
		free(curr->value);
		free(curr);
	}
}

int	builtin_unset(t_cmd *cmd, t_env **env)
{
	int		i;
	int		ret;

	if (!cmd->args[1])
		return (0);
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (!is_valid_env_key(cmd->args[i]))
		{
			print_error("unset", cmd->args[i], "not a valid identifier");
			ret = 1;
		}
		else
			remove_env_var(env, cmd->args[i]);
		i++;
	}
	return (ret);
}
