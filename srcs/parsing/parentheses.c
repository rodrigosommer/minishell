/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*execute_group(char *group_content, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;
	int			result;
	int			saved_exit_status;

	saved_exit_status = shell->last_exit_status;
	tokens = tokenize(group_content);
	if (!tokens)
	{
		shell->last_exit_status = saved_exit_status;
		return (ft_strdup("false"));
	}
	commands = parse(tokens, shell);
	free_tokens(tokens);
	if (!commands)
	{
		shell->last_exit_status = saved_exit_status;
		return (ft_strdup("false"));
	}
	result = execute_pipeline(commands, shell);
	free_commands(commands);
	shell->last_exit_status = saved_exit_status;
	if (result == 0)
		return (ft_strdup("true"));
	return (ft_strdup("false"));
}

static char	*extract_parentheses_content(char *start, char *end)
{
	int		len;
	char	*content;

	len = end - start - 1;
	if (len <= 0)
		return (ft_strdup(""));
	content = malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, start + 1, len + 1);
	return (content);
}

static char	*find_unquoted_paren(char *input, char paren)
{
	char	*p;

	p = input;
	while (*p)
	{
		if (*p == paren && !is_inside_quotes(input, p))
			return (p);
		p++;
	}
	return (NULL);
}

static char	*replace_parentheses_group(char *result, char *start, char *end,
		t_shell *shell)
{
	char	*group_content;
	char	*replacement;
	char	*new_input;
	char	*temp;

	group_content = extract_parentheses_content(start, end);
	replacement = execute_group(group_content, shell);
	temp = ft_substr(result, 0, start - result);
	new_input = ft_strjoin(temp, replacement);
	free(temp);
	temp = new_input;
	new_input = ft_strjoin(temp, end + 1);
	free(temp);
	free(group_content);
	free(replacement);
	return (new_input);
}

char	*preprocess_parentheses(char *input, t_shell *shell)
{
	char	*result;
	char	*start;
	char	*end;

	if (!find_unquoted_paren(input, '('))
		return (ft_strdup(input));
	result = ft_strdup(input);
	while (find_unquoted_paren(result, '('))
	{
		start = find_unquoted_paren(result, '(');
		end = find_unquoted_paren(start, ')');
		if (!end)
			break ;
		start = replace_parentheses_group(result, start, end, shell);
		free(result);
		result = start;
	}
	return (result);
}
