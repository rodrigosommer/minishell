/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:24:38 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:24:39 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int builtin_exit(t_cmd *cmd, t_data *data)
{
    int exit_code;
    
    ft_putendl_fd("exit", STDERR_FILENO);
    if (!cmd->args[1])
        exit_code = data->exit_status;
    else if (cmd->args[2])
        return (print_error("exit", NULL, "too many arguments"), 1);
    else if (!ft_isdigit_str(cmd->args[1]))
    {
        print_error("exit", cmd->args[1], "numeric argument required");
        exit_code = 2;
    }
    else
        exit_code = ft_atoi(cmd->args[1]);
    
    // Add this line before exit to ensure complete cleanup
    cleanup_shell(data);
    exit(exit_code);
}
