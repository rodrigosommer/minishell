/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:47:36 by hjuma             #+#    #+#             */
/*   Updated: 2024/11/06 10:37:42 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *str, ...);
void	ft_print_char(char c, int *size);
void	ft_print_str(char *str, int *size);
void	ft_print_nbr(int nbr, int *size);
void	ft_print_nbr_u(unsigned int nbr, int *size);
void	ft_print_ptr(void *ptr, int *size);
void	ft_print_hex(unsigned long num, const char *hex, int *size);

#endif
