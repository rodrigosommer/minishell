/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*init_command_args(t_token *tokens)
{
	t_command	*cmd;
	int			arg_count;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	ft_memset(cmd->args, 0, sizeof(char *) * (arg_count + 1));
	return (cmd);
}

static int	parse_tokens_loop(t_token **tokens, t_command *cmd,
			t_shell *shell, int *i)
{
	while (*tokens && (*tokens)->type != TOKEN_PIPE
		&& (*tokens)->type != TOKEN_AND && (*tokens)->type != TOKEN_OR
		&& (*tokens)->type != TOKEN_RPAREN)
	{
		if ((*tokens)->type == TOKEN_WORD)
			process_word_token(tokens, cmd, shell, i);
		else if (handle_redirect_token(tokens, cmd) == -1)
			return (-1);
		else
		{
			*tokens = (*tokens)->next;
			continue ;
		}
	}
	return (0);
}

t_command	*parse_command(t_token **tokens, t_shell *shell)
{
	t_command	*cmd;
	int			i;

	cmd = init_command_args(*tokens);
	if (!cmd)
		return (NULL);
	i = 0;
	if (parse_tokens_loop(tokens, cmd, shell, &i) == -1)
	{
		free_commands(cmd);
		return (NULL);
	}
	cmd->args[i] = NULL;
	remove_empty_args(cmd);
	expand_command_wildcards(cmd);
	return (cmd);
}
