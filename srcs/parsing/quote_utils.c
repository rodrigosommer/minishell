/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:04:22 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/26 17:04:24 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdbool.h>

// Forward declaration for expand_env_var
static char *expand_env_var(char *str, int *i, t_env *env);

typedef enum {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote_state;

static t_quote_state get_quote_state(char c, t_quote_state current)
{
    if (c == '\'' && current != DOUBLE_QUOTE)
        return current == SINGLE_QUOTE ? NO_QUOTE : SINGLE_QUOTE;
    if (c == '\"' && current != SINGLE_QUOTE)
        return current == DOUBLE_QUOTE ? NO_QUOTE : DOUBLE_QUOTE;
    return current;
}

static char *expand_env_var(char *str, int *i, t_env *env)
{
    int start = ++(*i); // Skip the $
    
    // Find end of variable name
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_')) {
        (*i)++;
    }
    
    char *var_name = ft_substr(str, start, *i - start);
    if (!var_name) return NULL;
    
    char *var_value = get_env_value(env, var_name);
    free(var_name);
    
    if (!var_value) var_value = "";
    
    // Replace the variable reference with its value
    char *before = ft_substr(str, 0, start - 1);
    char *after = ft_strdup(str + *i);
    char *result = ft_strjoin(before, var_value);
    
    free(before);
    if (after) {
        char *tmp = ft_strjoin(result, after);
        free(result);
        free(after);
        result = tmp;
    }
    
    (*i) = start - 1; // Position after replacement
    return result;
}

char *process_quotes_and_expand(char *token, t_env *env, int exit_status)
{
    if (!token) return NULL;
    
    t_quote_state state = NO_QUOTE;
    bool in_escape = false;
    char *result = ft_strdup(token);
    
    if (!result) return NULL;

    // First pass: expand variables (not in single quotes)
    for (int i = 0; result[i]; i++) {
        if (!in_escape && result[i] == '\\') {
            in_escape = true;
            continue;
        }

        state = get_quote_state(result[i], state);
        
        if (!in_escape && state != SINGLE_QUOTE && result[i] == '$') {
            char *expanded = NULL;
            if (result[i+1] == '?') {
                char *exit_str = ft_itoa(exit_status);
                expanded = ft_strreplace(result + i, "$?", exit_str);
                free(exit_str);
                i = 0; // Reset after replacement
            } else if (ft_isalpha(result[i+1]) || result[i+1] == '_') {
                int j = i;
                expanded = expand_env_var(result, &j, env);
                i = 0; // Reset after expansion
            }
            
            if (expanded) {
                free(result);
                result = expanded;
                if (!result) return NULL;
            }
        }
        in_escape = false;
    }

    // Second pass: remove syntactic quotes but preserve those that are part of the content
    char *final = ft_calloc(ft_strlen(result) + 1, sizeof(char));
    int j = 0;
    state = NO_QUOTE;
    in_escape = false;

    for (int i = 0; result[i]; i++) {
        if (!in_escape && result[i] == '\\') {
            in_escape = true;
            final[j++] = result[i];
            continue;
        }

        t_quote_state new_state = get_quote_state(result[i], state);
        if (new_state != state) {
            // Quote state changed - only remove if it's a syntactic quote
            if (state == NO_QUOTE) {
                // Opening quote - don't copy it if it's syntactic
                state = new_state;
            } else {
                // Closing quote - don't copy it if it's syntactic
                state = NO_QUOTE;
            }
        } else {
            // Regular character or quote within quotes
            final[j++] = result[i];
        }
        in_escape = false;
    }

    free(result);
    return final;
}
