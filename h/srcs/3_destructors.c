/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: terabu <terabu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 11:56:02 by terabu            #+#    #+#             */
/*   Updated: 2023/05/03 14:50:43 by terabu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	finalize(t_node *node, int wstatus)
{
	loop_node_delete_heredoc(node);
	set_wstatus(wstatus);
}

void	free_token(t_token *tok)
{
	if (tok)
		free_token(tok->next);
	else
		return ;
	if (tok->word)
		free(tok->word);
	free(tok);
}

void	free_nodelist(t_node *node)
{
	if (node == NULL)
		return ;
	free_token(node->args);
	free_token(node->filename);
	free_token(node->delimiter);
	free_nodelist(node->redirects);
	free_nodelist(node->next);
	free(node);
}

void	free_argv(char **args)
{
	size_t	i;

	if (args == NULL)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	set_wstatus(int wstatus)
{
	if (wstatus == SIGINT || wstatus == SIGQUIT)
	{
		g_global.status = 128 + wstatus;
		if (wstatus == SIGQUIT)
			put_error_msg_endl("Quit: 3");
	}
	else
		g_global.status = WEXITSTATUS(wstatus);
}
