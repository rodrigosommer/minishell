#include "../../includes/minishell.h"

static char	*get_env_path(void)
{
	t_environ	*env;

	env = g_global.env_head;
	while (env)
	{
		if (!ft_strcmp(env->name, "PATH"))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*get_cmd(char *cmd)
{
	int		i;
	char	**path_array;
	char	*r_str_path;

	path_array = ft_split(get_env_path(), ':');
	if (!path_array)
		return (NULL);
	i = 0;
	cmd = ft_strjoin("/", cmd);
	while (path_array[i])
	{
		r_str_path = ft_strjoin(path_array[i++], cmd);
		if (!access(r_str_path, X_OK))
		{
			free_argv(path_array);
			return (r_str_path);
		}
		free(r_str_path);
	}
	free_argv(path_array);
	return (NULL);
}

static char	*get_path(char *cmd)
{
	if (!access(cmd, X_OK))
		return (cmd);
	return (NULL);
}

char	*get_cmd_array(char *cmd_line)
{
	char	*r_str_array;

	if (cmd_line == NULL || ft_strlen(cmd_line) == 0)
		return (NULL);
	r_str_array = NULL;
	if (ft_strchr(cmd_line, '/'))
		r_str_array = get_path(cmd_line);
	else
		r_str_array = get_cmd(cmd_line);
	return (r_str_array);
}
