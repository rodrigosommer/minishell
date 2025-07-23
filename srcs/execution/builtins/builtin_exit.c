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
    int in_pipeline = 0;

    // Check if we're in a pipeline by looking for next command
    if (cmd->next != NULL) {
        in_pipeline = 1;
    }

    // Handle too many arguments
    if (cmd->args[1] && cmd->args[2]) {
        print_error("exit", NULL, "too many arguments");
        return (1);  // Return 1 to indicate error (too many args)
    }

    // Handle no arguments
    if (!cmd->args[1]) {
        if (!in_pipeline) {
            ft_putstr_fd("exit\n", STDERR_FILENO); // Print "exit" only for no args
        }
        exit_code = data->exit_status;
    }
    // Handle single argument
    else {
        char *arg = cmd->args[1];
        // Check if the argument is numeric (including + or -)
        if (!ft_isdigit_str(arg) && !((arg[0] == '+' || arg[0] == '-') && ft_isdigit_str(arg + 1))) {
            print_error("exit", arg, "numeric argument required");
            exit_code = 2;
        } else {
            exit_code = ft_atoi(arg); // No "exit" print for valid numeric args
        }
    }

    cleanup_shell(data);
    exit(exit_code);
}
