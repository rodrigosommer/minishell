/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:52:38 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/10 14:41:16 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	pathname_check(char *str)
{
	t_stat	tmp_stat;

	if (stat(str, &tmp_stat) == 0)
		return (0);
	else
		return (1);
}

static int	cd_home(void)
{
	char	*home_dir;

	home_dir = getenv("HOME");
	if (home_dir == NULL)
	{
		put_error_msg_endl("cd: could not get HOME directory");
		return (1);
	}
	if (chdir(home_dir) == -1)
	{
		put_error_msg_endl("cd: could not change to HOME directory");
		return (1);
	}
	return (0);
}

static int	change_directory(char **argv)
{
	if (chdir(argv[1]) == -1)
	{
		put_error_msg_endl("cd: No such file or directory");
		return (1);
	}
	return (0);
}

int	bi_cd_only(void)
{
	if (cd_home() == 1)
	{
		g_global.status = 1;
		return (-1);
	}
	return (0);
}

int	bi_cd(char **argv, int argc)
{
	g_global.status = 0;
	if (argc == 1)
		return (bi_cd_only());
	else
	{
		if (pathname_check(argv[1]) == 0)
		{
			if (change_directory(argv) == 1)
			{
				g_global.status = 1;
				return (-1);
			}
		}
		else
		{
			g_global.status = 1;
			put_error_msg_endl("cd: No such file or directory");
		}
	}
	return (0);
}
