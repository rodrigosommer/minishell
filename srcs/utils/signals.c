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

void handle_sigint(int sig)
{
    (void)sig;
    g_signal = 1;
    
    // Reset the readline state
    rl_replace_line("", 0);
    rl_done = 1;
    
    // Write newline and flush output
    write(STDOUT_FILENO, "\n", 1);
    
    // Reset terminal attributes
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ECHO | ECHONL | ICANON | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_child_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // Reset terminal attributes before changing signal handlers
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    sa_int.sa_handler = SIG_DFL;
    sa_int.sa_flags = SA_RESTART;
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = SIG_DFL;
    sa_quit.sa_flags = SA_RESTART;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
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
