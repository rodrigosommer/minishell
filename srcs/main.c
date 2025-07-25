/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:25 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:18:27 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal = 0;
t_data					*g_shell_data = NULL;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	g_shell_data = &data;
	(void)argc;
	(void)argv;
	setup_terminal();
	init_shell(&data, envp);
	setup_signals();
	run_shell(&data);
	cleanup_shell(&data);
	reset_terminal();
	return (data.exit_status);
}

void	init_shell(t_data *data, char **envp)
{
	data->env = init_env(envp);
	data->cmd = NULL;
	data->input = NULL;
	data->exit_status = 0;
	data->original_stdin = dup(STDIN_FILENO);
	data->original_stdout = dup(STDOUT_FILENO);
}

void	run_shell(t_data *data)
{
	char	*trimmed;
	int		ret;
	char	*ret_str;

	while (1)
	{
		g_signal = 0;
		data->input = readline(PROMPT);
		debug_log("run_shell", "Input received", data->input);
		if (!data->input)
		{
			debug_log("run_shell", "EOF received, exiting", NULL);
			break ;
		}
		trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
		if (!trimmed || !*trimmed)
		{
			debug_log("run_shell", "Empty input, skipping", NULL);
			free(data->input);
			free(trimmed);
			continue ;
		}
		free(trimmed);
		if (data->input[0] != '\0')
			add_history(data->input);
		data->cmd = parse_input(data->input, data->env, data);
		debug_log("run_shell", "Command parsed", data->cmd ? "Valid" : "NULL");
		if (data->cmd)
		{
			ret = execute_command(data->cmd, data->env, data);
			ret_str = ft_itoa(ret);
			debug_log("run_shell", "Command executed, status", ret_str);
			free(ret_str);
			data->exit_status = ret;
			free_commands(data->cmd);
			data->cmd = NULL;
		}
		free(data->input);
		data->input = NULL;
	}
}

void	cleanup_shell(t_data *data)
{
	struct termios	terminal_settings;

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
	if (data->original_stdin != -1)
	{
		close(data->original_stdin);
	}
	if (data->original_stdout != -1)
	{
		close(data->original_stdout);
	}
	if (tcgetattr(STDIN_FILENO, &terminal_settings) == 0)
	{
		terminal_settings.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &terminal_settings);
	}
}
