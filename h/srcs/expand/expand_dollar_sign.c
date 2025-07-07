#include "../../includes/minishell.h"

static void	last_exit_status(char **new_word)
{
	char	*str;
	char	*begin;

	str = ft_itoa(g_global.status);
	begin = str;
	while (*begin != '\0')
		append_char(new_word, *begin++);
	free(str);
}

static char	*t_environ_getenv(char *name, t_environ *env)
{
	char	*value;

	value = NULL;
	while (env != NULL)
	{
		if (ft_strcmp(env->name, name) == 0)
		{
			value = env->value;
			break ;
		}
		env = env->next;
	}
	return (value);
}

static void	expand_env(char **new_word, char *p, t_environ *env)
{
	char	*value;

	if (*p == '?')
	{
		last_exit_status(new_word);
		return ;
	}
	value = t_environ_getenv(p, env);
	if (value == NULL)
	{
		append_char(new_word, '\0');
		return ;
	}
	while (*value != '\0')
		append_char(new_word, *value++);
	return ;
}

static int	handle_dollar_sign(char **p, char **exp_tmp)
{
	(*p)++;
	if (**p == ' ' || **p == '\t' || **p == '\0')
		return (1);
	else
	{
		if (**p == '?')
			append_char(exp_tmp, *((*p)++));
		else
		{
			while (**p && !is_metacharacter(**p) && **p != '\"' && **p != '\''
				&& **p != DOLLAR_SIGN)
				append_char(exp_tmp, *((*p)++));
		}
	}
	return (0);
}

void	dollar_sign(char **p, char **new_word, t_environ *env)
{
	char	*exp_tmp;

	exp_tmp = NULL;
	while (**p && !is_metacharacter(**p) && **p != '\"' && **p != '\'')
	{
		if (**p == DOLLAR_SIGN)
		{
			if (handle_dollar_sign(p, &exp_tmp) == 1)
				append_char(new_word, '$');
			if (exp_tmp)
			{
				expand_env(new_word, exp_tmp, env);
				free(exp_tmp);
				exp_tmp = NULL;
			}
		}
		else
			break ;
	}
}
