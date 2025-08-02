/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 11:40:43 by rsommer           #+#    #+#             */
/*   Updated: 2024/11/03 11:40:50 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "ft_printf.h"

int	ft_putstr(char *str)
{
	if (!str)
		return (ft_putstr("(null)"));
	return (write(1, str, ft_strlen(str)));
}

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_print_hex(unsigned long addr)
{
	int	res;

	res = 0;
	if (addr > 15)
	{
		res += ft_print_hex(addr / 16);
	}
	res += ft_putchar("0123456789abcdef"[addr % 16]);
	return (res);
}

int	ft_putpointer(void *ptr)
{
	unsigned long	addr;
	int				res;

	if (!ptr)
		return (ft_putstr("(nil)"));
	addr = (unsigned long)ptr;
	res = ft_putstr("0x");
	if (addr == 0)
	{
		return (res + ft_putchar('0'));
	}
	return (res + ft_print_hex(addr));
}

int	ft_putnbr(int number)
{
	char	*num;
	int		len;

	len = 0;
	num = ft_itoa(number);
	len += ft_putstr(num);
	free(num);
	return (len);
}
