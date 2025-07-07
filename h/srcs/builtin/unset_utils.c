/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 21:56:13 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/07 15:04:24 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_first_variable(t_environ **environ)
{
	t_environ	*tmp;

	tmp = *environ;
	*environ = (*environ)->next;
	g_global.env_head = *environ;
	free(tmp->name);
	free(tmp->value);
	free(tmp);
}

void	remove_variable(t_environ *environ, t_environ *var)
{
	t_environ	*tmp;

	if (environ->name == var->name)
	{
		remove_first_variable(&environ);
		return ;
	}
	while (environ->next != NULL)
	{
		if (environ->next->name == var->name)
		{
			tmp = environ->next;
			environ->next = environ->next->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			break ;
		}
		environ = environ->next;
	}
	return ;
}
