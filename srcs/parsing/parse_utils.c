/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_command(t_command **commands, t_command *new_cmd)
{
	t_command	*current;

	if (!*commands)
	{
		*commands = new_cmd;
		return ;
	}
	current = *commands;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

void	free_commands(t_command *commands)
{
	t_command	*temp;
	int			i;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
				free(temp->args[i++]);
			free(temp->args);
		}
		free_redirects(temp->redirects);
		free(temp);
	}
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->operator = TOKEN_EOF;
	cmd->next = NULL;
	return (cmd);
}

t_command	*parse(t_token *tokens, t_shell *shell)
{
	t_command	*commands;
	t_command	*cmd;

	commands = NULL;
	while (tokens)
	{
		cmd = parse_command(&tokens, shell);
		if (!cmd)
		{
			free_commands(commands);
			return (NULL);
		}
		add_command(&commands, cmd);
		if (tokens && (tokens->type == TOKEN_PIPE
				|| tokens->type == TOKEN_AND || tokens->type == TOKEN_OR))
		{
			cmd->operator = tokens->type;
			tokens = tokens->next;
		}
	}
	return (commands);
}

int	handle_redirect_token(t_token **tokens, t_command *cmd)
{
	t_redirect	*redirect;
	t_token		*current;

	current = *tokens;
	if (current->type >= TOKEN_REDIRECT_IN
		&& current->type <= TOKEN_REDIRECT_HEREDOC)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type != TOKEN_WORD)
		{
			return (-1);
		}
		redirect = create_redirect(current->type, (*tokens)->value);
		if (!redirect)
		{
			return (-1);
		}
		add_redirect(&cmd->redirects, redirect);
		return (0);
	}
	return (-1);
}
