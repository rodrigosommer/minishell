/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:30:00 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:15 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	size_t	i;

	if (!s)
		return (0);
	str = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (0);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*
char to_upper_at_even(unsigned int i, char c) {
    if (i % 2 == 0 && c >= 'a' && c <= 'z')
        return (c - 32); // Convert to uppercase
    return c;
}

int main() {
    char *original = "hello world";
    char *transformed = ft_strmapi(original, to_upper_at_even);

    if (transformed) {
        printf("Original: %s\n", original);
        printf("Transformed: %s\n", transformed);
        free(transformed);
    }

    return 0;
}*/
