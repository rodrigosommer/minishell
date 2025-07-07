/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:30:42 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:35:55 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (0);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len <= s_len - start)
		str = malloc(sizeof(char) * (len + 1));
	else
		str = malloc(sizeof(char) * (s_len - start + 1));
	if (!str)
		return (0);
	i = 0;
	while (s[start] && i < len)
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}
/*
int main() {
    const char *original =  "Hello, World!";
    unsigned int start = 7; // Starting index
    size_t length = 5; // Length of the substring

    char *substring = ft_substr(original, start, length);
    if (substring) {
        printf("Substring: '%s'\n", substring); // Should print: 'World'
        free(substring); // Free allocated memory
    } else {
        printf("Failed to create substring.\n");
    }

    return 0;
}
*/
