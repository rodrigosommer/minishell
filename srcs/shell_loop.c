/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;
	char		*preprocessed;

	preprocessed = preprocess_parentheses(input, shell);
	tokens = tokenize(preprocessed);
	free(preprocessed);
	if (!tokens)
		return ;
	commands = parse(tokens, shell);
	free_tokens(tokens);
	if (!commands)
		return ;
	shell->last_exit_status = execute_pipeline(commands, shell);
	free_commands(commands);
}

void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			process_input(input, shell);
		}
		free(input);
	}
}
