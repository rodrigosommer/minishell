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
t_data *g_shell_data = NULL;

int main(int argc, char **argv, char **envp)
{
    t_data data;
    g_shell_data = &data;  // Set global pointer

    (void)argc;
    (void)argv;
    setup_terminal();  // Add this line
    init_shell(&data, envp);
    setup_signals();
    run_shell(&data);
    cleanup_shell(&data);
    reset_terminal();  // Add this line
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
    char *trimmed;
    
    while (1)
    {
        // Reset signal flag
        g_signal = 0;
        
        // Get input
        data->input = readline(PROMPT);
        if (!data->input)
        {
            ft_putendl_fd("exit", STDERR_FILENO);
            break;
        }

        // Skip empty lines
        trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
        if (!trimmed || !*trimmed)
        {
            free(data->input);
            free(trimmed);
            continue;
        }
        free(trimmed);

        // Add to history if not empty
        if (data->input[0] != '\0')
            add_history(data->input);

        // Parse and execute
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
    // Free input buffer if it exists
    if (data->input) {
        free(data->input);
        data->input = NULL;
    }

    // Free command structures if they exist
    if (data->cmd) {
        free_commands(data->cmd);
        data->cmd = NULL;
    }

    // Free environment variables if they exist
    if (data->env) {
        free_env(data->env);
        data->env = NULL;
    }

    // Clear command history
    rl_clear_history();

    // Free readline internal state
    rl_free_line_state();
    rl_cleanup_after_signal();
    clean_readline();

    // Restore original standard input/output if they were saved
    if (data->original_stdin != -1) {
        close(data->original_stdin);
    }
    if (data->original_stdout != -1) {
        close(data->original_stdout);
    }

    // Restore terminal to original state
    struct termios terminal_settings;
    if (tcgetattr(STDIN_FILENO, &terminal_settings) == 0) {
        terminal_settings.c_lflag |= ECHOCTL; // Ensure control characters are echoed
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_settings);
    }
}
