/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:27:09 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:11 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}
/*
int main()
{
    const char str1[] = "Hello, World!";
    const char str2[] = "Hello, World!";
    const char str3[] = "Hello, Wxrld!";

    // Compare str1 and str2 (they are equal)
    int result1 = ft_memcmp(str1, str2, sizeof(str1));
    if (result1 == 0)
        printf("str1 and str2 are equal.\n");
    else
        printf("str1 and str2 are not equal.\n");
}
*/
