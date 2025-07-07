#include "../../includes/minishell.h"

void	tokenize_error(char *location, char **rest, char *line, int flg)
{
	g_global.syntax_error = true;
	perror_prefix();
	if (flg < 0)
	{
		put_error_msg("syntax error: ");
		put_error_msg_endl(location);
	}
	else
	{
		put_error_msg("syntax error near unexpected character `");
		put_error_char(*line);
		put_error_msg("' in ");
		put_error_msg_endl(location);
	}
	g_global.status = 258;
	while (*line)
		line++;
	*rest = line;
}

void	parse_error(t_token **rest, t_token *tok)
{
	g_global.syntax_error = true;
	perror_prefix();
	put_error_msg_endl("syntax error !!");
	g_global.status = 258;
	while (tok && tok->kind != TK_EOF)
		tok = tok->next;
	*rest = tok;
}
