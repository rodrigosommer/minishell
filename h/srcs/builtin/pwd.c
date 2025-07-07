/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:15:31 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/05 19:56:17 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	bi_pwd(void)
{
	char	path[PATH_MAX];

	g_global.status = 0;
	if (!getcwd(path, sizeof(path)))
	{
		put_error_msg_endl("getcwd: getcwd fail");
		return (-1);
	}
	printf("%s\n", path);
	return (0);
}
