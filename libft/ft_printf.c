/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:46:07 by hjuma             #+#    #+#             */
/*   Updated: 2024/11/06 13:02:08 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_printf_type(va_list ap, char type, int *size)
{
	if (type == 'c')
		ft_print_char((char)va_arg(ap, int), size);
	else if (type == 's')
		ft_print_str(va_arg(ap, char *), size);
	else if (type == 'p')
		ft_print_ptr(va_arg(ap, void *), size);
	else if (type == 'd' || type == 'i')
		ft_print_nbr(va_arg(ap, int), size);
	else if (type == 'u')
		ft_print_nbr_u(va_arg(ap, unsigned int), size);
	else if (type == 'x')
		ft_print_hex(va_arg(ap, unsigned int), "0123456789abcdef", size);
	else if (type == 'X')
		ft_print_hex(va_arg(ap, unsigned int), "0123456789ABCDEF", size);
	else if (type == '%')
		ft_print_char('%', size);
}

int	ft_printf(const char *str, ...)
{
	va_list	ap;
	int		size;

	va_start (ap, str);
	size = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			if (*str)
			{
				ft_printf_type(ap, *str, &size);
				str++;
			}
		}
		else
		{
			ft_print_char(*str, &size);
			str++;
		}
	}
	va_end (ap);
	return (size);
}
