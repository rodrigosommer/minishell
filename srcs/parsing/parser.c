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

// Forward declarations to avoid implicit declaration errors
static t_cmd *create_command_node(char **args);
static void add_token_to_args(t_cmd *cmd, char *token);

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

static int handle_redirection(t_cmd *cmd, char **tokens, int *i, t_env *env, t_data *data)
{
    char *file;
    int fd = -1; // Initialize fd to avoid uninitialized use

    if (!tokens[*i + 1]) {
        print_error("syntax error", NULL, "missing file for redirection");
        data->exit_status = 2; // Syntax error exit code
        debug_log("handle_redirection", "Missing file for redirection", tokens[*i]);
        return 1;
    }

    // Expand variables and process quotes for the filename
    debug_log("handle_redirection", "Processing filename", tokens[*i + 1]);
    file = process_quotes_and_expand(tokens[*i + 1], env, data->exit_status);
    if (!file || !*file) {
        print_error("redirection", tokens[*i + 1], "invalid filename after expansion");
        debug_log("handle_redirection", "Invalid filename after expansion", tokens[*i + 1]);
        free(file);
        data->exit_status = 1;
        return 1;
    }
    debug_log("handle_redirection", "Expanded filename", file);

    if (!ft_strcmp(tokens[*i], ">")) {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            print_error("redirection", file, strerror(errno));
            debug_log("handle_redirection", "Failed to open output file", file);
            free(file);
            data->exit_status = 1;
            return 1;
        }
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
        cmd->out_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], ">>")) {
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            print_error("redirection", file, strerror(errno));
            debug_log("handle_redirection", "Failed to open append file", file);
            free(file);
            data->exit_status = 1;
            return 1;
        }
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
        cmd->out_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], "<")) {
        fd = open(file, O_RDONLY);
        if (fd == -1) {
            print_error("redirection", file, strerror(errno));
            debug_log("handle_redirection", "Failed to open input file", file);
            free(file);
            data->exit_status = 1;
            return 1;
        }
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        cmd->in_fd = fd;
    }
    else if (!ft_strcmp(tokens[*i], "<<")) {
        fd = handle_heredoc(file);
        if (fd == -1) {
            debug_log("handle_redirection", "Heredoc failed", file);
            free(file);
            data->exit_status = 1;
            return 1;
        }
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        cmd->in_fd = fd;
    }

    debug_log("handle_redirection", "Redirection successful, fd", ft_itoa(fd));
    free(file);
    free(tokens[*i]);
    free(tokens[*i + 1]);
    tokens[*i] = NULL;
    tokens[*i + 1] = NULL;
    *i += 1;

    return 0;
}

t_cmd *parse_input(char *input, t_env *env, t_data *data)
{
    char **tokens = tokenize_input(input);
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    int i = 0;

    debug_log("parse_input", "Input", input);
    if (!tokens) {
        debug_log("parse_input", "Tokenization failed", NULL);
        data->exit_status = 1;
        return NULL;
    }

    current = create_command_node(NULL);
    head = current;

    while (tokens[i]) {
        debug_log("parse_input", "Processing token", tokens[i]);
        if (!ft_strcmp(tokens[i], "|")) {
            current->next = create_command_node(NULL);
            current = current->next;
            i++;
            continue;
        }

        if (is_redirection_token(tokens[i])) {
            if (handle_redirection(current, tokens, &i, env, data)) {
                debug_log("parse_input", "Redirection handling failed", tokens[i]);
                free_array(tokens);
                free_commands(head);
                data->exit_status = 2;
                return NULL;
            }
            continue;
        }

        char *expanded = expand_variables(tokens[i], env, data->exit_status);
        if (!expanded) {
            debug_log("parse_input", "Variable expansion failed", tokens[i]);
            free_array(tokens);
            free_commands(head);
            data->exit_status = 1;
            return NULL;
        }
        
        debug_log("parse_input", "Expanded token", expanded);
        if (!*expanded) { // Handle empty expanded token
            debug_log("parse_input", "Empty expanded token", tokens[i]);
            free(expanded);
            i++;
            continue;
        }
        
        process_token_quotes(&expanded, env, data);
        debug_log("parse_input", "Processed quotes", expanded);
        
        add_token_to_args(current, expanded);
        free(expanded);
        i++;
    }

    free_array(tokens);
    debug_log("parse_input", "Parsing completed", NULL);
    return head;
}

static t_cmd *create_command_node(char **args)
{
    t_cmd *node = ft_calloc(1, sizeof(t_cmd));
    if (!node)
        return NULL;
    node->args = args;
    node->in_fd = STDIN_FILENO;
    node->out_fd = STDOUT_FILENO;
    node->next = NULL;
    return node;
}

static void add_token_to_args(t_cmd *cmd, char *token)
{
    int i = 0;
    if (!cmd->args)
    {
        cmd->args = ft_calloc(2, sizeof(char *));
        if (!cmd->args)
            return;
        cmd->args[0] = ft_strdup(token);
        return;
    }
    while (cmd->args[i])
        i++;
    char **new_args = ft_calloc(i + 2, sizeof(char *));
    if (!new_args)
        return;
    for (int j = 0; j < i; j++)
        new_args[j] = cmd->args[j];
    new_args[i] = ft_strdup(token);
    free(cmd->args);
    cmd->args = new_args;
}
