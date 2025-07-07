/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susasaki <susasaki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 19:49:00 by susasaki          #+#    #+#             */
/*   Updated: 2023/05/07 15:06:42 by susasaki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_sign(char *str, int *i)
{
	int	flag;

	flag = 1;
	if (str[*i] == '-')
	{
		flag = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
		(*i)++;
	return (flag);
}

static unsigned long long	parse_number(char *str, int *i)
{
	unsigned long long	tmp_ll;

	tmp_ll = 0;
	while ('0' <= str[*i] && str[*i] <= '9')
	{
		tmp_ll *= 10;
		tmp_ll += str[*i] - '0';
		(*i)++;
	}
	return (tmp_ll);
}

int	int_overflow_check(char *str, long *num)
{
	int					flag;
	int					i;
	unsigned long long	tmp_ll;

	i = 0;
	flag = check_sign(str, &i);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	tmp_ll = parse_number(str, &i);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] != '\0')
		return (1);
	if ((LONG_MAX < tmp_ll && flag == 1) || 9223372036854775808ull < tmp_ll)
		return (1);
	tmp_ll *= flag;
	*num = tmp_ll;
	return (0);
}
