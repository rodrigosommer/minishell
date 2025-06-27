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

static void process_token_quotes(char **token, t_env *env, t_data *data)
{
    char *processed = process_quotes_and_expand(*token, env, data->exit_status);
    if (!processed) {
        free(*token);
        *token = NULL;
        return;
    }
    
    free(*token);
    *token = processed;
}

static int	is_redirection_token(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">") 
		|| !ft_strcmp(token, "<<") || !ft_strcmp(token, ">>"));
}

static int handle_redirection(t_cmd *cmd, char **tokens, int *i)
{
    char *file;
    int fd;

    if (!tokens[*i + 1]) {
        print_error("syntax error", NULL, "missing file for redirection");
        return 1;
    }

    file = tokens[*i + 1];

    if (!ft_strcmp(tokens[*i], ">")) {
        fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644);
        if (fd == -1) {
            print_error(file, NULL, strerror(errno));
            return 1;
        }
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
        cmd->out_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], ">>")) {
        fd = open(file, O_WRONLY|O_CREAT|O_APPEND, 0644);
        if (fd == -1) {
            print_error(file, NULL, strerror(errno));
            return 1;
        }
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
        cmd->out_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], "<")) {
        fd = open(file, O_RDONLY);
        if (fd == -1) {
            print_error(file, NULL, strerror(errno));
            return 1;
        }
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        cmd->in_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], "<<")) {
        fd = handle_heredoc(file);
        if (fd == -1)
            return 1;
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        cmd->in_fd = fd;
    }

    // Remove redirection tokens from command args
    free(tokens[*i]);
    free(tokens[*i + 1]);
    tokens[*i] = NULL;
    tokens[*i + 1] = NULL;
    *i += 1;

    return 0;
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

t_cmd *parse_input(char *input, t_env *env, t_data *data)
{
    char **tokens = tokenize_input(input);
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    int i = 0;

    if (!tokens)
        return NULL;

    current = create_command_node(NULL);
    head = current;

    while (tokens[i])
    {
        if (!ft_strcmp(tokens[i], "|"))
        {
            current->next = create_command_node(NULL);
            current = current->next;
            i++;
            continue;
        }

        if (is_redirection_token(tokens[i]))
        {
            if (handle_redirection(current, tokens, &i)) {
                free_array(tokens);
                free_commands(head);
                return NULL;
            }
            continue;
        }

        char *expanded = expand_variables(tokens[i], env, data->exit_status);
        if (!expanded) {
            free_array(tokens);
            free_commands(head);
            return NULL;
        }
        
        // Remove quotes after variable expansion
        process_token_quotes(&expanded, env, data);
        
        add_token_to_args(current, expanded);
        free(expanded);
        i++;
    }

    free_array(tokens);
    return head;
}
