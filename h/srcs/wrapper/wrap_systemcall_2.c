	#include "../../includes/minishell.h"

void	do_unlink(char *str)
{
	if (unlink(str) < 0)
		fatal_error("unlink");
}

void	do_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
		fatal_error("pipe");
}
