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

static int get_exit_code(t_cmd *cmd, t_data *data, int *exit_code)
{
    if (cmd->args[1] && cmd->args[2]) {
        print_error("exit", NULL, "too many arguments");
        debug_log("builtin_exit", "Too many arguments", NULL);
        return (1);
    }
    
    if (cmd->args[1]) {
        char *arg = cmd->args[1];
        if ((arg[0] == '+' || arg[0] == '-') && ft_isdigit(arg[1])) {
            *exit_code = ft_atoi(arg);
            debug_log("builtin_exit", "Parsed exit code", ft_itoa(*exit_code));
            return (0);
        }
        if (!ft_isdigit_str(arg)) {
            print_error("exit", arg, "numeric argument required");
            debug_log("builtin_exit", "Non-numeric argument", arg);
            *exit_code = 2;
            return (0);
        }
        *exit_code = ft_atoi(arg);
        debug_log("builtin_exit", "Parsed exit code", ft_itoa(*exit_code));
    } else {
        *exit_code = data->exit_status;
        debug_log("builtin_exit", "Using previous exit status", ft_itoa(*exit_code));
    }
    return (0);
}

int builtin_exit(t_cmd *cmd, t_data *data)
{
    int exit_code;

    ft_putendl_fd("exit", STDERR_FILENO);
    debug_log("builtin_exit", "Executing exit command", cmd->args[1] ? cmd->args[1] : "NULL");
    if (get_exit_code(cmd, data, &exit_code)) {
        data->exit_status = 1;
        debug_log("builtin_exit", "Exit status set to", "1");
        return (1); // Return 1 for too many arguments
    }
    cleanup_shell(data);
    debug_log("builtin_exit", "Exiting with code", ft_itoa(exit_code));
    exit(exit_code);
}
