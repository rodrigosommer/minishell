/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:47:07 by hjuma             #+#    #+#             */
/*   Updated: 2024/11/04 09:40:05 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_hex(unsigned long num, const char *base, int *size)
{
	if (num == 0)
	{
		ft_print_char('0', size);
		return ;
	}
	if (num >= 16)
		ft_print_hex(num / 16, base, size);
	ft_print_char(base[num % 16], size);
}
