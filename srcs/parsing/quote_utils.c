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
    
    (*i) = start - 1 + ft_strlen(var_value); // Position after replacement
    return result;
}

char *process_quotes_and_expand(char *token, t_env *env, int exit_status)
{
    if (!token) return NULL;
    
    char *result = ft_strdup(token);
    if (!result) return NULL;

    // First pass: expand variables (not in single quotes)
    int i = 0;
    bool in_single_quote = false;
    bool in_double_quote = false;
    bool escaped = false;
    
    while (result[i]) {
        if (!escaped && result[i] == '\\') {
            escaped = true;
            i++;
            continue;
        }

        if (!escaped) {
            if (result[i] == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (result[i] == '\"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            }
        }
        
        if (!escaped && !in_single_quote && result[i] == '$') {
            char *expanded = NULL;
            if (result[i+1] == '?') {
                char *exit_str = ft_itoa(exit_status);
                char *new_result = ft_calloc(ft_strlen(result) + ft_strlen(exit_str), sizeof(char));
                if (new_result) {
                    ft_strlcpy(new_result, result, i + 1);
                    ft_strlcat(new_result, exit_str, ft_strlen(result) + ft_strlen(exit_str));
                    ft_strlcat(new_result, result + i + 2, ft_strlen(result) + ft_strlen(exit_str));
                    free(result);
                    result = new_result;
                    i += ft_strlen(exit_str) - 1;
                }
                free(exit_str);
            } else if (ft_isalpha(result[i+1]) || result[i+1] == '_') {
                int j = i;
                expanded = expand_env_var(result, &j, env);
                if (expanded) {
                    free(result);
                    result = expanded;
                    i = j; // Continue from where expansion left off
                    in_single_quote = false;
                    in_double_quote = false;
                    escaped = false;
                    continue;
                }
            }
        }
        escaped = false;
        i++;
    }

    // Second pass: remove quotes
    char *final = ft_calloc(ft_strlen(result) + 1, sizeof(char));
    if (!final) {
        free(result);
        return NULL;
    }
    
    int j = 0;
    i = 0;
    in_single_quote = false;
    in_double_quote = false;
    escaped = false;

    while (result[i]) {
        if (!escaped && result[i] == '\\') {
            escaped = true;
            if (in_single_quote) {
                final[j++] = result[i]; // Keep backslashes in single quotes
            }
            i++;
            continue;
        }

        if (!escaped) {
            if (result[i] == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
                i++; // Skip the quote character
                continue;
            } else if (result[i] == '\"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
                i++; // Skip the quote character
                continue;
            }
        }

        // Copy the character
        final[j++] = result[i];
        escaped = false;
        i++;
    }

    free(result);
    return final;
}
