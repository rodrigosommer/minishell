/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_init_environ_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 21:56:04 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/07 14:18:31 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char	**environ;

int	first_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
	{
		i++;
	}
	if (i == 0)
		return (-1);
	return (i);
}

int	latter_strlen(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
		return (-1);
	i++;
	while (str[i] != '\0')
	{
		i++;
		res++;
	}
	return (res);
}

t_environ	*create_new_environ_node(char *env_str)
{
	char		*name;
	char		*value;
	t_environ	*new_node;

	name = make_name(env_str);
	value = make_value(env_str);
	new_node = environ_node_new(name, value);
	new_node->name = name;
	new_node->value = value;
	return (new_node);
}

t_environ	*populate_environ_list(t_environ *env_list, char **tmp)
{
	t_environ	*new;
	t_environ	*current;
	int			i;
	int			flag;

	i = 0;
	flag = 0;
	while (tmp[i] != NULL)
	{
		new = create_new_environ_node(tmp[i]);
		if (flag == 0)
		{
			env_list = new;
			current = env_list;
			g_global.env_head = current;
			flag = 1;
		}
		else
		{
			current->next = new;
			current = current->next;
		}
		i++;
	}
	return (env_list);
}

t_environ	*init_environ_list(void)
{
	char		**tmp;
	t_environ	*env_list;

	if (environ[0] == NULL)
	{
		g_global.env_head = NULL;
		return (NULL);
	}
	tmp = environ;
	env_list = NULL;
	env_list = populate_environ_list(env_list, tmp);
	return (env_list);
}
