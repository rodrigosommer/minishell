/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:26:59 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:27:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_array(char **array)
{
    if (!array)
        return;
        
    for (int i = 0; array[i]; i++)
    {
        if (array[i])
        {
            free(array[i]);
            array[i] = NULL;
        }
    }
    free(array);
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        if (env->key)
            free(env->key);
        if (env->value)
            free(env->value);
        free(env);
        env = tmp;
    }
}

void free_commands(t_cmd *cmd)
{
    t_cmd *tmp;

    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->args)
            free_array(cmd->args);
        if (cmd->in_fd != STDIN_FILENO)
            close(cmd->in_fd);
        if (cmd->out_fd != STDOUT_FILENO)
            close(cmd->out_fd);
        free(cmd);
        cmd = tmp;
    }
}
