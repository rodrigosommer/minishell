/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:25 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/18 13:27:37 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_shell(&data, envp);
	setup_signals();
	run_shell(&data);
	cleanup_shell(&data);
	return (data.exit_status);
}

void	init_shell(t_data *data, char **envp)
{
	data->env = init_env(envp);
	data->cmd = NULL;
	data->input = NULL;
	data->exit_status = 0;
}

void	run_shell(t_data *data)
{
	while (1)
	{
		data->input = readline(PROMPT);
		if (!data->input)
		{
			ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		if (*data->input)
		{
			add_history(data->input);
			data->cmd = parse_input(data->input, data->env, data);
			if (data->cmd)
			{
				execute_command(data->cmd, data->env, data);
				free_commands(data->cmd);
				data->cmd = NULL;
			}
		}
		free(data->input);
		data->input = NULL;
	}
}

void	cleanup_shell(t_data *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->cmd)
	{
		free_commands(data->cmd);
		data->cmd = NULL;
	}
	if (data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	rl_clear_history();
	rl_free_line_state();
	rl_cleanup_after_signal();
	clean_readline();
}
