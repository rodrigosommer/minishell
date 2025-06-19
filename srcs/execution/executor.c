/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:22:14 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/19 15:56:14 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_builtin(t_cmd *cmd)
{
	if (!cmd->args[0])
		return (0);
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (1);
	return (0);
}

static int	execute_builtin(t_cmd *cmd, t_env **env, t_data *data)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd));
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd, *env));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd, env));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd, env));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(*env));
	if (!ft_strcmp(cmd->args[0], "exit"))
		builtin_exit(cmd, data);
	return (0);
}

static char	**env_to_array(t_env *env)
{
	char	**envp;
	int		count;
	int		i;
	t_env	*temp;

	count = 0;
	temp = env;
	while (temp)
	{
		if (temp->value)
			count++;
		temp = temp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	temp = env;
	while (temp && i < count)
	{
		if (temp->value)
		{
			envp[i] = ft_strjoin3(temp->key, "=", temp->value);
			if (!envp[i])
				return (free_array(envp), NULL);
			i++;
		}
		temp = temp->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	has_redirection(t_cmd *cmd)
{
	return (cmd->in_fd != STDIN_FILENO || cmd->out_fd != STDOUT_FILENO);
}

int	execute_command(t_cmd *cmd, t_env *env, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*path;

	if (!cmd->args[0])
		return (0);
	// Handle built-in without fork if it's a single command
	if (is_builtin(cmd) && !cmd->next && !has_redirection(cmd))
		return (execute_builtin(cmd, &env, data));
	pid = fork();
	if (pid == -1)
	{
		print_error("fork", NULL, strerror(errno));
		return (1);
	}

	if (pid == 0)
	{
		handle_redirections(cmd);
		if (cmd->next)
			handle_pipes(cmd, env, data);
		if (is_builtin(cmd))
			exit(execute_builtin(cmd, &env, data));

		envp = env_to_array(env);
		path = find_command_path(cmd->args[0], env);
		if (!path)
		{
			print_error(cmd->args[0], NULL, "command not found");
			free_array(envp);
			exit(127);
		}
		execve(path, cmd->args, envp);
		free(path);
		free_array(envp);
		print_error(cmd->args[0], NULL, strerror(errno));
		exit(127);
	}

	// Parent process: wait and store status
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);

	return (0);
}


char	*find_command_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (!cmd || ft_strchr(cmd, '/')) // already a path
		return (ft_strdup(cmd));

	path_env = get_env_value(env, "PATH"); // you must have a get_env_value()
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':'); // use your own split function
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
