/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:56:03 by hjuma             #+#    #+#             */
/*   Updated: 2025/06/25 13:56:16 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_global	g_global;

static	bool	check_syntax_error(t_token *token, t_node *node, int flg)
{
	if (flg == 1 && g_global.syntax_error)
	{
		free_token(token);
		return (true);
	}
	else if (flg == 2 && g_global.syntax_error)
	{
		free_nodelist(node);
		return (true);
	}
	else if (flg == 3 && g_global.flg_redir)
	{
		loop_node_delete_heredoc(node);
		free_nodelist(node);
		g_global.flg_redir = 0;
		return (true);
	}
	else
		return (false);
}

static void	line_matches_cmd(char *line, t_environ *environ)
{
	t_token	*token;
	t_node	*node;

	token = tokenize(line);
	node = NULL;
	if (check_syntax_error(token, node, 1))
		return ;
	node = parse(token);
	free_token(token);
	if (check_syntax_error(token, node, 2))
		return ;
	expand(node, environ);
	check_heredoc(node, environ);
	if (check_syntax_error(token, node, 3))
		return ;
	execution(node, environ);
	free_nodelist(node);
	return ;
}

int	main(void)
{
	char		*input;
	t_environ	*environ;

	g_global.syntax_error = false;
	signal(SIGQUIT, signal_handler);
	init_environ_list();
	while (1)
	{
		environ = g_global.env_head;
		signal(SIGINT, signal_handler);
		input = readline("\x1b[32mminishell> \x1b[0m");
		if (input == NULL)
			break ;
		else if (ft_strlen(input) == 0)
			;
		else
		{
			add_history(input);
			line_matches_cmd(input, environ);
		}
		free(input);
	}
	return (g_global.status);
}
