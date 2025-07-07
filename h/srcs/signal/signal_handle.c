#include "../../includes/minishell.h"

void	signal_handler_waiting_input(int sig)
{
	if (sig == SIGINT)
	{
		signal(SIGINT, SIG_DFL);
		g_global.status = 130;
		signal(SIGINT, signal_handler_waiting_input);
	}
	else if (sig == SIGQUIT)
	{
		signal(SIGQUIT, SIG_DFL);
		g_global.status = 131;
		signal(SIGQUIT, signal_handler_waiting_input);
	}
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
		signal_c_cmd();
	else if (sig == SIGQUIT)
		signal_backslash();
	return ;
}

void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_global.flg_redir = 1;
		g_global.status = 1;
	}
	return ;
}
