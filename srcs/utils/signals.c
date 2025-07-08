/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:25:51 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:25:52 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <termios.h>

volatile sig_atomic_t g_command_executing = 0;

int is_executing_command(void)
{
    return g_command_executing;
}

void handle_sigint(int sig)
{
    (void)sig;
    g_signal = 1;
    
    // Only handle display if we're at the prompt
    if (!g_command_executing) {
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Let the child process handle the display
}

void setup_signals(void)
{
    struct sigaction sa;

    // Parent process signals
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;  // Parent ignores SIGQUIT
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);  // Child gets default SIGQUIT handler
}

void setup_terminal(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);  // Don't print control chars like ^D
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void reset_terminal(void)
{
    struct termios term;
    
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        return; // Silently fail if we can't get terminal attributes
    }
    
    // Reset to sensible defaults
    term.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG);
    term.c_lflag &= ~(ECHOCTL); // Don't echo control characters
    
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
