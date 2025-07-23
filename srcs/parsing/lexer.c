/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:53 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:18:54 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int is_redirection_char(char c)
{
    return (c == '<' || c == '>');
}

static int count_tokens(char *input)
{
    int count = 0;
    int in_quote = 0;
    int in_dquote = 0;
    int in_token = 0;
    int i = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_dquote)
            in_quote = !in_quote;
        else if (input[i] == '\"' && !in_quote)
            in_dquote = !in_dquote;
        
        if (!in_quote && !in_dquote)
        {
            if (ft_isspace(input[i]))
            {
                if (in_token)
                {
                    count++;
                    in_token = 0;
                }
            }
            else if (is_redirection_char(input[i]))
            {
                if (in_token)
                {
                    count++;
                    in_token = 0;
                }
                // Count redirection operator
                count++;
                if (input[i] == input[i + 1])
                    i++; // Skip second char of >> or <<
            }
            else if (input[i] == '|')
            {
                if (in_token)
                {
                    count++;
                    in_token = 0;
                }
                count++; // Count pipe
            }
            else
            {
                in_token = 1;
            }
        }
        else
        {
            in_token = 1;
        }
        i++;
    }
    if (in_token)
        count++;
    return count;
}

char **tokenize_input(char *input)
{
    char **tokens;
    int count = count_tokens(input);
    int i = 0;
    int token_idx = 0;
    int len;
    int in_quote = 0;
    int in_dquote = 0;

    tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return NULL;

    while (i < (int)ft_strlen(input) && token_idx < count)
    {
        // Skip whitespace
        while (ft_isspace(input[i]))
            i++;
        
        if (!input[i])
            break;
        
        len = 0;
        in_quote = 0;
        in_dquote = 0;
        
        // Handle redirection operators
        if (!in_quote && !in_dquote && is_redirection_char(input[i]))
        {
            if (input[i] == input[i + 1])
                len = 2; // >> or <<
            else
                len = 1; // > or <
        }
        // Handle pipe
        else if (!in_quote && !in_dquote && input[i] == '|')
        {
            len = 1;
        }
        // Handle regular tokens
        else
        {
            while (input[i + len] && 
                   (in_quote || in_dquote || 
                    (!ft_isspace(input[i + len]) && 
                     !is_redirection_char(input[i + len]) && 
                     input[i + len] != '|')))
            {
                if (input[i + len] == '\'' && !in_dquote)
                    in_quote = !in_quote;
                else if (input[i + len] == '\"' && !in_quote)
                    in_dquote = !in_dquote;
                len++;
            }
        }
        
        if (len > 0)
        {
            tokens[token_idx] = ft_substr(input, i, len);
            if (!tokens[token_idx])
            {
                free_array(tokens);
                return NULL;
            }
            token_idx++;
        }
        i += len;
    }
    tokens[token_idx] = NULL;
    return tokens;
}
