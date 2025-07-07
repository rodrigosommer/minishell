/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 19:42:50 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/07 14:52:54 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_nflag(char **argv, int *i)
{
	int	nflag;
	int	j;

	nflag = 0;
	j = 1;
	if (argv[1] && argv[1][0] == '-')
	{
		while (argv[1][j])
		{
			if (argv[1][j] == 'n')
				nflag = 1;
			else
			{
				nflag = 0;
				break ;
			}
			j++;
		}
	}
	if (nflag == 1)
		(*i)++;
	return (nflag);
}

void	print_arguments(char **argv, int i, int nflag)
{
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (nflag == 0)
		printf("\n");
}

int	bi_echo(char **argv)
{
	int	nflag;
	int	i;

	i = 1;
	nflag = check_nflag(argv, &i);
	print_arguments(argv, i, nflag);
	g_global.status = 0;
	return (0);
}
