/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_wildcard_matches(char *arg)
{
	char	**results;
	int		count;

	results = expand_wildcards(arg);
	if (!results)
		return (1);
	count = 0;
	while (results[count])
		count++;
	free_wildcard_results(results);
	return (count);
}

static int	count_total_args(t_command *cmd)
{
	int	i;
	int	total_args;

	i = 0;
	total_args = 0;
	while (cmd->args[i])
	{
		if (!has_quotes(cmd->args[i]) && ft_strchr(cmd->args[i], '*'))
			total_args += count_wildcard_matches(cmd->args[i]);
		else
			total_args++;
		i++;
	}
	return (total_args);
}

static int	copy_wildcard_results(char **new_args, char *arg, int k)
{
	char	**results;
	int		j;

	results = expand_wildcards(arg);
	if (!results)
	{
		new_args[k] = arg;
		return (k + 1);
	}
	j = 0;
	while (results[j])
	{
		new_args[k] = ft_strdup(results[j]);
		j++;
		k++;
	}
	free_wildcard_results(results);
	free(arg);
	return (k);
}

void	expand_command_wildcards(t_command *cmd)
{
	char	**new_args;
	int		total_args;
	int		i;
	int		k;

	total_args = count_total_args(cmd);
	new_args = malloc(sizeof(char *) * (total_args + 1));
	if (!new_args)
		return ;
	i = 0;
	k = 0;
	while (cmd->args[i])
	{
		if (!has_quotes(cmd->args[i]) && ft_strchr(cmd->args[i], '*'))
			k = copy_wildcard_results(new_args, cmd->args[i], k);
		else
			new_args[k++] = cmd->args[i];
		i++;
	}
	new_args[k] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}
