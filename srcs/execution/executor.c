/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:22:14 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:22:20 by rsommer          ###   ########.fr       */
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

int	execute_command(t_cmd *cmd, t_env *env, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**envp;

	if (!cmd->args[0])
		return (0);
	if (is_builtin(cmd) && !cmd->next)
		return (execute_builtin(cmd, &env, data));
	pid = fork();
	if (pid == -1)
		return (print_error("fork", NULL, strerror(errno)), 1);
	if (pid == 0)
	{
		handle_redirections(cmd);
		if (cmd->next)
			handle_pipes(cmd, env, data);
		if (is_builtin(cmd))
			exit(execute_builtin(cmd, &env, data));
		envp = env_to_array(env);
		execve(cmd->args[0], cmd->args, envp);
		free_array(envp);
		print_error(cmd->args[0], NULL, strerror(errno));
		exit(127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	return (0);
}
