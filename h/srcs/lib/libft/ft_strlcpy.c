/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:29:41 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:21 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size)
	{
		while (src[i] && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}
/*
int main()
{
	char buffer[10];
	const char *str = "Hello, world ";
	size_t result = ft_strlcpy(buffer, str, sizeof(buffer));

	printf("Concatenated string: %s\n", buffer);  // Output: "Hello, world!"
	printf("Total length: %zu\n", result);         
	// Output: 13 (length of "Hello, world!")
	return 0;
}
*/
