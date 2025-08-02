/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE
		&& tokens->type != TOKEN_AND && tokens->type != TOKEN_OR)
	{
		if (tokens->type == TOKEN_WORD)
		{
			count++;
			tokens = tokens->next;
		}
		else if (tokens->type >= TOKEN_REDIRECT_IN
			&& tokens->type <= TOKEN_REDIRECT_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
				tokens = tokens->next;
		}
		else
			tokens = tokens->next;
	}
	return (count);
}

int	process_word_token(t_token **tokens, t_command *cmd,
	t_shell *shell, int *i)
{
	char	*expanded;
	int		was_quoted;

	was_quoted = has_quotes((*tokens)->value);
	expanded = expand_variables((*tokens)->value, shell);
	cmd->args[*i] = remove_quotes(expanded);
	free(expanded);
	(*i)++;
	*tokens = (*tokens)->next;
	return (0);
}

void	remove_empty_args(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (!*cmd->args[i])
		{
			free(cmd->args[i]);
			while (cmd->args[i + 1])
			{
				cmd->args[i] = cmd->args[i + 1];
				i++;
			}
			cmd->args[i] = NULL;
			i = 0;
		}
		else
			i++;
	}
}
