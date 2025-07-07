/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:27:25 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:05 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*d;
	const char	*s;

	d = (char *)dst;
	s = (const char *)src;
	if (!d && !s)
		return (dst);
	if (d < s)
	{
		while (len--)
			*d++ = *s++;
	}
	else
	{
		d += len;
		s += len;
		while (len--)
			*(--d) = *(--s);
	}
	return (dst);
}
/*/
int main()
{
    char buffer[50] = "Hello, World!";
    
    // Non-overlapping case
    printf("Before memmove: %s\n", buffer);
    ft_memmove(buffer + 7, buffer, 6); // Move "Hello," to position 7
    printf("After memmove: %s\n", buffer); // Output: Hello, Hello,!
}
*/
