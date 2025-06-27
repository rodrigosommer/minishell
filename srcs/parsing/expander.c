/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:47 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:21:59 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *expand_env_var(char *str, int *i, t_env *env)
{
    char *var_name;
    char *var_value;
    char *result;
    int start = ++(*i); // Skip the $
    
    while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        (*i)++;
    
    var_name = ft_substr(str, start, *i - start);
    if (!var_name)
        return NULL;
    
    var_value = get_env_value(env, var_name);
    free(var_name);
    
    if (!var_value)
        var_value = "";
    
    // Construct the new string with the variable expanded
    char *before = ft_substr(str, 0, start - 1);
    char *after = ft_strdup(str + *i);
    result = ft_strjoin(before, var_value);
    free(before);
    
    if (after) {
        char *tmp = ft_strjoin(result, after);
        free(result);
        free(after);
        result = tmp;
    }
    
    *i = start - 1; // Position after replacement
    return result;
}

char *expand_variables(char *token, t_env *env, int exit_status)
{
    if (!token)
        return NULL;

    char *result = ft_strdup(token);
    if (!result)
        return NULL;

    int i = 0;
    int in_squote = 0;
    int in_dquote = 0;
    int escaped = 0;

    while (result[i]) {
        if (!escaped && result[i] == '\\') {
            escaped = 1;
            i++;
            continue;
        }

        if (!escaped) {
            if (result[i] == '\'' && !in_dquote)
                in_squote = !in_squote;
            else if (result[i] == '\"' && !in_squote)
                in_dquote = !in_dquote;
        }

        if (!in_squote && !escaped && result[i] == '$') {
            if (result[i+1] == '?') {
                char *exit_str = ft_itoa(exit_status);
                char *new = ft_strreplace(result + i, "$?", exit_str);
                free(exit_str);
                free(result);
                result = new ? new : ft_strdup("");
                if (!result)
                    return NULL;
                i = -1; // Reset index after replacement
                in_squote = 0;
                in_dquote = 0;
            }
            else if (ft_isalpha(result[i+1]) || result[i+1] == '_') {
                char *expanded = expand_env_var(result, &i, env);
                free(result);
                result = expanded;
                if (!result)
                    return NULL;
                i = -1; // Reset index after expansion
                in_squote = 0;
                in_dquote = 0;
            }
        }
        escaped = 0;
        i++;
    }
    return result;
}
