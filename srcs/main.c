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

int main(int argc, char **argv, char **envp)
{
    t_data data;

    (void)argc;
    (void)argv;
    setup_terminal();  // Add this line
    init_shell(&data, envp);
    setup_signals();
    run_shell(&data);
    cleanup_shell(&data);
    restore_terminal();  // Add this line
    return (data.exit_status);
}

void init_shell(t_data *data, char **envp)
{
    data->env = init_env(envp);
    data->cmd = NULL;
    data->input = NULL;
    data->exit_status = 0;
    data->original_stdin = dup(STDIN_FILENO);  // Add this
    data->original_stdout = dup(STDOUT_FILENO); // Add this
}

void run_shell(t_data *data)
{
    while (1)
    {
        data->input = readline(PROMPT);
        if (!data->input)
        {
            ft_putendl_fd("exit", STDERR_FILENO);
            break;
        }
        
        // Skip empty or whitespace-only input
        char *trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
        if (!trimmed || !*trimmed)
        {
            free(data->input);
            free(trimmed);
            data->input = NULL;
            continue;
        }
        free(trimmed);
        
        // Only process if we have real content
        add_history(data->input);
        data->cmd = parse_input(data->input, data->env, data);
        if (data->cmd)
        {
            execute_command(data->cmd, data->env, data);
            free_commands(data->cmd);
            data->cmd = NULL;
        }
        free(data->input);
        data->input = NULL;
    }
}

void cleanup_shell(t_data *data)
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
    
    // Add these lines to clean up readline/internal allocations
    rl_free_line_state();
    rl_cleanup_after_signal();
    clean_readline();
}
