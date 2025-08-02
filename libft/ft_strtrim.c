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

static size_t	get_start_index(char const *s1, char const *set, size_t len)
{
	size_t	start;

	start = 0;
	while (start < len && ft_strchr(set, s1[start]))
		start++;
	return (start);
}

static size_t	get_end_index(char const *s1, char const *set, size_t len,
			size_t start)
{
	size_t	end;

	end = len;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	return (end);
}

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
	len = ft_strlen(s1);
	if (len == 0)
		return (ft_strdup(""));
	start = get_start_index(s1, set, len);
	if (start >= len)
		return (ft_strdup(""));
	end = get_end_index(s1, set, len, start);
	len = end - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1 + start, len + 1);
	return (result);
}
