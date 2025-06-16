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

#include "../../../includes/minishell.h"

int	builtin_unset(t_cmd *cmd, t_env **env)
{
	int		i;
	t_env	*prev;
	t_env	*current;
	int		ret;

	if (!cmd->args[1])
		return (0);
	ret = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_env_key(cmd->args[i]))
		{
			print_error("unset", cmd->args[i], "not a valid identifier");
			ret = 1;
			i++;
			continue ;
		}
		prev = NULL;
		current = *env;
		while (current && ft_strcmp(current->key, cmd->args[i]))
		{
			prev = current;
			current = current->next;
		}
		if (current)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
		}
		i++;
	}
	return (ret);
}
