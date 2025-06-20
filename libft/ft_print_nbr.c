/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_nbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:46:48 by hjuma             #+#    #+#             */
/*   Updated: 2024/11/04 09:47:43 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_nbr(int n, int *size)
{
	if (n == -2147483648)
	{
		ft_print_str("-2147483648", size);
		return ;
	}
	if (n < 0)
	{
		ft_print_char('-', size);
		n = -n;
	}
	if (n >= 10)
		ft_print_nbr(n / 10, size);
	ft_print_char((n % 10) + '0', size);
}
