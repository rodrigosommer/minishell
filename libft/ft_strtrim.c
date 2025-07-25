/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:24:00 by rsommer           #+#    #+#             */
/*   Updated: 2024/10/22 11:27:11 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len;
	char	*result;

	if (!s1 || !set)
		return (NULL);
	if (!*s1)
		return (ft_strdup(""));
	start = 0;
	len = ft_strlen(s1);
	if (len == 0)
		return (ft_strdup(""));
	while (start < len && ft_strchr(set, s1[start]))
		start++;
	if (start >= len)
		return (ft_strdup(""));
	end = len;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1 + start, len + 1);
	return (result);
}
