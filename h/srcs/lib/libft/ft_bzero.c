/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:24:10 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/22 10:22:00 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*ptr;
	size_t	i;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
/*
int main()
{
    char str[20] = "Hello, World!";

    printf("before: %s\n", str);
    ft_bzero(str, sizeof(str));

    // Print the string after calling ft_bzero
    printf("After ft_bzero: %s\n", str);

    return 0;
}
*/
