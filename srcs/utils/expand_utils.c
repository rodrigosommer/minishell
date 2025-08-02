/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **env)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

char	*handle_var_expansion(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = get_var_name_from_str(str, i);
	var_value = expand_var(var_name, shell);
	free(var_name);
	return (var_value);
}

char	*handle_char_addition(char *str, int *i)
{
	char	*temp;

	temp = ft_substr(str, *i, 1);
	(*i)++;
	return (temp);
}

char	*process_char_with_quotes(char *str, int *i, t_shell *shell)
{
	int	in_single_quote;
	int	in_double_quote;
	int	j;

	in_single_quote = 0;
	in_double_quote = 0;
	j = 0;
	while (j < *i)
	{
		if (str[j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[j] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		j++;
	}
	if (str[*i] == '\'' && !in_double_quote)
		return (handle_char_addition(str, i));
	else if (str[*i] == '"' && !in_single_quote)
		return (handle_char_addition(str, i));
	else if (str[*i] == '$' && str[*i + 1] && !in_single_quote
		&& (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'
			|| str[*i + 1] == '?'))
		return (handle_var_expansion(str, i, shell));
	else
		return (handle_char_addition(str, i));
}
