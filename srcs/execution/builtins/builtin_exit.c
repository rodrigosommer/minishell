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

int	builtin_exit(t_cmd *cmd, t_data *data)
{
	int		exit_code;
	int		in_pipeline;
	char	*arg;

	in_pipeline = 0;
	if (cmd->next != NULL)
	{
		in_pipeline = 1;
	}
	if (cmd->args[1] && cmd->args[2])
	{
		print_error("exit", NULL, "too many arguments");
		return (1);
	}
	if (!cmd->args[1])
	{
		if (!in_pipeline)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
		}
		exit_code = data->exit_status;
	}
	else
	{
		arg = cmd->args[1];
		if (!ft_isdigit_str(arg) && !((arg[0] == '+' || arg[0] == '-')
				&& ft_isdigit_str(arg + 1)))
		{
			print_error("exit", arg, "numeric argument required");
			exit_code = 2;
		}
		else
		{
			exit_code = ft_atoi(arg);
		}
	}
	cleanup_shell(data);
	exit(exit_code);
}
