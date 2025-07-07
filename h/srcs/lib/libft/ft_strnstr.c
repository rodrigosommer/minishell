/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:30:18 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:10 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *need, size_t len)
{
	size_t	i;
	size_t	j;

	if (*need == '\0')
		return ((char *)s);
	i = 0;
	while (s[i] && i < len)
	{
		j = 0;
		while (s[i + j] && need[j] && s[i + j] == need[j] && (i + j) < len)
			j++;
		if (j == ft_strlen(need))
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}
/*
int main() {
    const char *haystack = "Hello, world!";
    const char *needle = "world";
    const char *not_found = "moon";
    
    char *result = ft_strnstr(haystack, needle, 15);
    if (result) {
        printf("Found '%s' in haystack: %s\n", needle, result);
    } else {
        printf("'%s' not found in haystack.\n", needle);
    }

    result = ft_strnstr(haystack, not_found, 15);
    if (result) {
        printf("Found '%s' in haystack: %s\n", not_found, result);
    } else {
        printf("'%s' not found in haystack.\n", not_found);
    }

    return 0;
}
*/
