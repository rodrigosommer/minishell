#include "../../includes/minishell.h"

void	perror_prefix(void)
{
	put_error_msg(ERROR_PREFIX);
}

void	fatal_error(char *msg)
{
	perror_prefix();
	put_error_msg("Fatal Error: ");
	put_error_msg_endl(msg);
	exit(1);
}

void	assert_error(char *msg)
{
	perror_prefix();
	put_error_msg("Fatal Error: ");
	put_error_msg_endl(msg);
	exit(255);
}
