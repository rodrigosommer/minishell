/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 21:56:13 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/07 15:04:14 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_param_name(char *str)
{
	if (ft_strchr(str, '=') != NULL)
		return (0);
	return (1);
}

t_environ	*find_variable(t_environ *environ, char *str)
{
	t_environ	*tmp;

	tmp = environ;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, str) == 0)
		{
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	bi_unset(t_environ *environ, char **argv, int argc)
{
	t_environ	*var;

	g_global.status = 0;
	if (argc < 2)
		return (-1);
	if (argc > 2)
	{
		put_error_msg_endl("unset: too many arguments");
		g_global.status = 1;
		return (-1);
	}
	if (!is_valid_param_name(argv[1]))
	{
		put_error_msg_endl("unset: invalid parameter name");
		return (-1);
	}
	var = find_variable(environ, argv[1]);
	if (var == NULL)
	{
		put_error_msg_endl("unset: can't find variable");
		return (-1);
	}
	remove_variable(environ, var);
	return (0);
}
