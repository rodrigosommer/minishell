/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:34:47 by hjuma             #+#    #+#             */
/*   Updated: 2024/11/06 10:37:44 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_ptr(void *ptr, int *size)
{
	unsigned long	p;

	p = (unsigned long) ptr;
	if (p == 0)
	{
		ft_print_str("(nil)", size);
	}
	else
	{
		ft_print_str("0x", size);
		ft_print_hex(p, "0123456789abcdef", size);
	}
}
