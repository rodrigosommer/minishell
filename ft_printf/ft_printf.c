/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 11:39:33 by rsommer           #+#    #+#             */
/*   Updated: 2024/11/03 11:39:45 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_scan(char c, va_list args)
{
	if (c == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(args, char *)));
	else if (c == 'p')
		return (ft_putpointer(va_arg(args, void *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (c == 'u')
		return (ft_putstr(
				ft_convert(va_arg(args, unsigned int), "0123456789")));
	else if (c == '%')
		return (ft_putchar('%'));
	else if (c == 'x')
		return (ft_putstr(
				ft_convert(va_arg(args, unsigned int), "0123456789abcdef")));
	else if (c == 'X')
		return (ft_putstr(
				ft_convert(va_arg(args, unsigned int), "0123456789ABCDEF")));
	return (0);
}

char	*ft_convert(unsigned int num, char *set)
{
	static char	buffer[50];
	char		*ptr;
	int			base;

	base = ft_strlen(set);
	ptr = &buffer[49];
	*ptr = '\0';
	*--ptr = set[num % base];
	num /= base;
	while (num != 0)
	{
		*--ptr = set[num % base];
		num /= base;
	}
	return (ptr);
}

int	ft_printf(const char *sign, ...)
{
	va_list	args;
	int		i;
	int		len;

	va_start(args, sign);
	i = 0;
	len = 0;
	while (sign[i])
	{
		if (sign[i] == '%')
		{
			len += ft_scan(sign[i + 1], args);
			i++;
		}
		else
			len += write(1, &sign[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
