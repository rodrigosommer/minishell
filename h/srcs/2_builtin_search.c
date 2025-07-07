/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_builtin_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: terabu <terabu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:45:48 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/11 11:09:54 by terabu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_bi(char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	else if (ft_strcmp(cmd, "env") == 0)
		return (true);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	else if (ft_strcmp(cmd, "export") == 0)
		return (true);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	else
		return (false);
}

static int	search_bi_cmd_helper(int argc, char **argv, t_environ *environ)
{
	int	flag;

	flag = 0;
	if (ft_strcmp(argv[0], "pwd") == 0)
		flag = bi_pwd();
	else if (ft_strcmp(argv[0], "cd") == 0)
		flag = bi_cd(argv, argc);
	else if (ft_strcmp(argv[0], "echo") == 0)
		flag = bi_echo(argv);
	else if (ft_strcmp(argv[0], "env") == 0)
		flag = bi_env(argc, environ);
	else if (ft_strcmp(argv[0], "exit") == 0)
		flag = bi_exit(argv);
	else if (ft_strcmp(argv[0], "export") == 0)
		flag = bi_export(environ, argv, argc);
	else if (ft_strcmp(argv[0], "unset") == 0)
		flag = bi_unset(environ, argv, argc);
	else
		return (1);
	return (flag);
}

static int	execute_builtin_command(t_node *node, char **argv,
		t_environ *environ)
{
	int	flag;

	if (check_bi(argv[0]))
	{
		redirection(node->redirects);
		if (g_global.flg_redir == 1)
		{
			close_fd(node->redirects);
			return (-1);
		}
		flag = search_bi_cmd_helper((int)get_token_count(node->args), argv,
				environ);
		reset_redirect(node->redirects);
		return (flag);
	}
	return (1);
}

int	search_bi_cmd(t_node *node, t_environ *environ)
{
	int		flag;
	char	**argv;

	argv = token_list_to_array(node->args);
	if (argv == NULL)
	{
		return (0);
	}
	if (argv[0] == NULL)
	{
		if (node->redirects != NULL)
		{
			open_redir_file(node->redirects);
			close_fd(node->redirects);
		}
		free_argv(argv);
		return (0);
	}
	flag = execute_builtin_command(node, argv, environ);
	free_argv(argv);
	return (flag);
}
