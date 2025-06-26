/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:18:05 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/26 07:19:21 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_UTILS_H
# define EXECUTOR_UTILS_H

# include "minishell.h"

int         is_builtin(t_cmd *cmd);
int         execute_builtin(t_cmd *cmd, t_env **env, t_data *data);
char        **env_to_array(t_env *env);

#endif
