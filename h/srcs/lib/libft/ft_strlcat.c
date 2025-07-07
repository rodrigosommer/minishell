/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:29:30 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:23 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	i;

	if (!dst || !src)
		return (0);
	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size <= len_dst)
		return (len_src + size);
	i = 0;
	while (src[i] && (len_dst + i < size - 1))
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (len_dst + len_src);
}
/*
int main()
{
	char buffer[20] = "Hello, ";
	const char *str = "world!";
	size_t result = ft_strlcat(buffer, str, sizeof(buffer));

	printf("Concatenated string: %s\n", buffer);  // Output: "Hello, world!"
	printf("Total length: %zu\n", result);    
	     // Output: 13 (length of "Hello, world!")
	return 0;
}
*/
