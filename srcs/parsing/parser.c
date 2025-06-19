/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:17 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/19 15:50:17 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}

static int	handle_redirection(t_cmd *cmd, char **tokens, int *i)
{
	int		fd;
	char	*file;

	if (!tokens[*i + 1])
		return (print_error("parser", NULL, "syntax error"), 1);
	file = tokens[*i + 1];
	if (!ft_strcmp(tokens[*i], "<"))
		fd = open(file, O_RDONLY);
	else if (!ft_strcmp(tokens[*i], ">"))
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (!ft_strcmp(tokens[*i], ">>"))
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (!ft_strcmp(tokens[*i], "<<"))
		fd = handle_heredoc(file);
	else
		return (0);
	if (fd == -1)
		return (print_error(file, NULL, strerror(errno)), 1);
	if (!ft_strcmp(tokens[*i], "<") || !ft_strcmp(tokens[*i], "<<"))
		cmd->in_fd = fd;
	else
		cmd->out_fd = fd;
	(*i)++;
	return (0);
}

static t_cmd	*create_command_node(char **args)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

static void	add_token_to_args(t_cmd *cmd, char *token)
{
	char	**new_args;
	int		i;

	i = 0;
	if (!cmd->args)
	{
		cmd->args = malloc(sizeof(char *) * 2);
		if (cmd->args)
		{
			cmd->args[0] = ft_strdup(token);
			cmd->args[1] = NULL;
		}
		return ;
	}
	while (cmd->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	i = -1;
	while (cmd->args[++i])
		new_args[i] = cmd->args[i];
	new_args[i] = ft_strdup(token);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

t_cmd	*parse_input(char *input, t_env *env, t_data *data)
{
	char	**tokens;
	t_cmd	*head;
	t_cmd	*current;
	int		i;
	char	*expanded;

	tokens = tokenize_input(input);
	head = NULL;
	current = NULL;
	i = 0;
	if (!tokens)
		return (NULL);
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "|"))
		{
			if (!current || !current->args)
			{
				free_array(tokens);
				free_commands(head);
				return (NULL);
			}
			current->next = create_command_node(NULL);
			current = current->next;
			free(tokens[i]);
			i++;
			continue ;
		}
		if (is_redirection_token(tokens[i]))
		{
			if (!current)
				current = create_command_node(NULL);
			if (handle_redirection(current, tokens, &i))
			{
				free_array(tokens + i);
				free_commands(head);
				return (NULL);
			}
			free(tokens[i - 1]);
			continue ;
		}
		if (!current)
		{
			current = create_command_node(NULL);
			head = current;
		}
		expanded = expand_variables(tokens[i], env, data->exit_status);
		add_token_to_args(current, expanded);
		free(tokens[i]);
		free(expanded);
		i++;
	}
	free(tokens);
	return (head);
}
