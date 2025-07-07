/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:30:10 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:13 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
/*
int main() {
    const char *str1 = "hello";
    const char *str2 = "helLo";

    int result = ft_strncmp(str1, str2, 3);
    printf("Comparison of first 3 characters: %d\n", result);

    result = ft_strncmp(str1, str2, 5);
    printf("Comparison of first 5 characters: %d\n", result);

    return 0;
}
*/
