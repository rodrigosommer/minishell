#include "../../includes/minishell.h"

void	pipex_utils(t_node *node, int flag, t_environ *environ)
{
	if (flag == -1)
		exit(EXIT_FAILURE);
	else if (flag == 0)
		exit(EXIT_SUCCESS);
	else
		exec_cmd(node, environ);
}
