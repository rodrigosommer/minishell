/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:26:38 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:26:39 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// error_handling.c
#include "../../includes/minishell.h"
#include <fcntl.h>

void	debug_log(const char *function, const char *message, const char *value)
{
	int	fd;

	fd = open("minishell_debug.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return ;
	ft_putstr_fd("[DEBUG] ", fd);
	ft_putstr_fd((char *)function, fd);
	ft_putstr_fd(": ", fd);
	ft_putstr_fd((char *)message, fd);
	if (value)
	{
		ft_putstr_fd(": ", fd);
		ft_putstr_fd((char *)value, fd);
	}
	ft_putstr_fd("\n", fd);
	close(fd);
}

void	print_error(char *cmd, char *arg, char *error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	debug_log("print_error", "Error reported", error);
}

int	is_valid_env_key(char *key)
{
	int	i;

	i = 0;
	if (!key || !*key)
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
