/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:17 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:21:35 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <fcntl.h>

static t_cmd	*create_command_node(char **args);
static void		add_token_to_args(t_cmd *cmd, char *token);

static int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}

static int	handle_redirection(t_cmd *cmd, char **tokens, int *i, t_env *env,
		t_data *data, int in_pipeline)
{
	char	*file;
	int		fd;
	char	*fd_str;

	fd = -1;
	if (!tokens[*i + 1])
	{
		print_error("syntax error", NULL, "missing file for redirection");
		data->exit_status = 2;
		debug_log("handle_redirection", "Missing file for redirection",
			tokens[*i]);
		return (1);
	}
	debug_log("handle_redirection", "Processing filename", tokens[*i + 1]);
	file = process_quotes_and_expand(tokens[*i + 1], env, data->exit_status);
	if (!file || !*file)
	{
		print_error("redirection", tokens[*i + 1], "ambiguous redirect");
		debug_log("handle_redirection", "Invalid filename after expansion",
			tokens[*i + 1]);
		free(file);
		data->exit_status = 1;
		return (1);
	}
	debug_log("handle_redirection", "Expanded filename", file);
	if (!ft_strcmp(tokens[*i], ">"))
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			print_error(NULL, file, strerror(errno));
			debug_log("handle_redirection", "Failed to open output file", file);
			free(file);
			if (in_pipeline)
			{
				if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd > STDERR_FILENO)
					close(cmd->out_fd);
				cmd->out_fd = -1;
				(*i) += 2;
				return (1);
			}
			else
			{
				data->exit_status = 1;
				return (1);
			}
		}
		if (cmd->out_fd > STDERR_FILENO)
			close(cmd->out_fd);
		cmd->out_fd = fd;
	}
	else if (!ft_strcmp(tokens[*i], ">>"))
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			print_error(NULL, file, strerror(errno));
			debug_log("handle_redirection", "Failed to open append file", file);
			free(file);
			if (in_pipeline)
			{
				if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd > STDERR_FILENO)
					close(cmd->out_fd);
				cmd->out_fd = -1;
				(*i) += 2;
				return (1);
			}
			else
			{
				data->exit_status = 1;
				return (1);
			}
		}
		if (cmd->out_fd > STDERR_FILENO)
			close(cmd->out_fd);
		cmd->out_fd = fd;
	}
	else if (!ft_strcmp(tokens[*i], "<"))
	{
		fd = open(file, O_RDONLY);
		if (fd == -1)
		{
			print_error(NULL, file, strerror(errno));
			debug_log("handle_redirection", "Failed to open input file", file);
			free(file);
			if (in_pipeline)
			{
				if (cmd->in_fd != STDIN_FILENO && cmd->in_fd > STDERR_FILENO)
					close(cmd->in_fd);
				cmd->in_fd = -1;
				(*i) += 2;
				return (1);
			}
			else
			{
				data->exit_status = 1;
				return (1);
			}
		}
		if (cmd->in_fd > STDERR_FILENO)
			close(cmd->in_fd);
		cmd->in_fd = fd;
	}
	else if (!ft_strcmp(tokens[*i], "<<"))
	{
		fd = handle_heredoc(file);
		if (fd == -1)
		{
			debug_log("handle_redirection", "Heredoc failed", file);
			free(file);
			if (in_pipeline)
			{
				if (cmd->in_fd != STDIN_FILENO && cmd->in_fd > STDERR_FILENO)
					close(cmd->in_fd);
				cmd->in_fd = -1;
				(*i) += 2;
				return (1);
			}
			else
			{
				data->exit_status = 1;
				return (1);
			}
		}
		if (cmd->in_fd > STDERR_FILENO)
			close(cmd->in_fd);
		cmd->in_fd = fd;
	}
	fd_str = ft_itoa(fd);
	debug_log("handle_redirection", "Redirection successful, fd", fd_str);
	free(fd_str);
	free(file);
	(*i) += 2;
	return (0);
}

t_cmd	*parse_input(char *input, t_env *env, t_data *data)
{
	char	**tokens;
	t_cmd	*head;
	t_cmd	*current;
	int		i;
	int		in_pipeline;
	char	*expanded;
	char	*processed;

	tokens = tokenize_input(input);
	head = NULL;
	current = NULL;
	i = 0;
	debug_log("parse_input", "Input", input);
	if (!tokens)
	{
		debug_log("parse_input", "Tokenization failed", NULL);
		data->exit_status = 1;
		return (NULL);
	}
	current = create_command_node(NULL);
	if (!current)
	{
		free_array(tokens);
		return (NULL);
	}
	head = current;
	while (tokens[i])
	{
		debug_log("parse_input", "Processing token", tokens[i]);
		if (!ft_strcmp(tokens[i], "|"))
		{
			if (!current->args || !current->args[0])
			{
				print_error("syntax error", NULL, "unexpected token `|'");
				free_array(tokens);
				free_commands(head);
				data->exit_status = 2;
				return (NULL);
			}
			current->next = create_command_node(NULL);
			if (!current->next)
			{
				free_array(tokens);
				free_commands(head);
				return (NULL);
			}
			current = current->next;
			i++;
			continue ;
		}
		if (is_redirection_token(tokens[i]))
		{
			in_pipeline = 0;
			for (int j = i; tokens[j]; j++)
			{
				if (!ft_strcmp(tokens[j], "|"))
				{
					in_pipeline = 1;
					break ;
				}
			}
			if (handle_redirection(current, tokens, &i, env, data, in_pipeline))
			{
				debug_log("parse_input", "Redirection handling failed", NULL);
				if (!in_pipeline)
				{
					free_array(tokens);
					free_commands(head);
					return (NULL);
				}
				while (tokens[i] && ft_strcmp(tokens[i], "|"))
				{
					i++;
				}
				if (tokens[i] && !ft_strcmp(tokens[i], "|"))
				{
					current->next = create_command_node(NULL);
					if (!current->next)
					{
						free_array(tokens);
						free_commands(head);
						return (NULL);
					}
					current = current->next;
					i++;
				}
				continue ;
			}
			continue ;
		}
		expanded = expand_variables(tokens[i], env, data->exit_status);
		if (!expanded)
		{
			debug_log("parse_input", "Variable expansion failed", tokens[i]);
			free_array(tokens);
			free_commands(head);
			data->exit_status = 1;
			return (NULL);
		}
		debug_log("parse_input", "Expanded token", expanded);
		if (!*expanded)
		{
			debug_log("parse_input", "Empty expanded token", tokens[i]);
			free(expanded);
			i++;
			continue ;
		}
		processed = process_quotes_and_expand(expanded, env, data->exit_status);
		free(expanded);
		if (!processed)
		{
			debug_log("parse_input", "Quote processing failed", tokens[i]);
			free_array(tokens);
			free_commands(head);
			data->exit_status = 1;
			return (NULL);
		}
		debug_log("parse_input", "Processed quotes", processed);
		add_token_to_args(current, processed);
		free(processed);
		i++;
	}
	free_array(tokens);
	if (!head || !head->args || !head->args[0])
	{
		debug_log("parse_input", "No valid commands found", NULL);
		free_commands(head);
		return (NULL);
	}
	debug_log("parse_input", "Parsing completed", NULL);
	return (head);
}

static t_cmd	*create_command_node(char **args)
{
	t_cmd	*node;

	node = ft_calloc(1, sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->args = args;
	node->in_fd = STDIN_FILENO;
	node->out_fd = STDOUT_FILENO;
	node->next = NULL;
	return (node);
}

static void	add_token_to_args(t_cmd *cmd, char *token)
{
	int		i;
	char	**new_args;

	i = 0;
	if (!cmd->args)
	{
		cmd->args = ft_calloc(2, sizeof(char *));
		if (!cmd->args)
			return ;
		cmd->args[0] = ft_strdup(token);
		return ;
	}
	while (cmd->args[i])
		i++;
	new_args = ft_calloc(i + 2, sizeof(char *));
	if (!new_args)
		return ;
	for (int j = 0; j < i; j++)
		new_args[j] = cmd->args[j];
	new_args[i] = ft_strdup(token);
	free(cmd->args);
	cmd->args = new_args;
}
