/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:33:03 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/16 00:33:16 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strreplace(char *str, char *find, char *replace)
{
	char	*pos;
	char	*new;
	size_t	len;
	size_t	find_len;
	size_t	replace_len;

	if (!str || !find || !replace)
		return (NULL);
	find_len = ft_strlen(find);
	if (find_len == 0)
		return (ft_strdup(str));
	pos = ft_strnstr(str, find, ft_strlen(str));
	if (!pos)
		return (ft_strdup(str));
	replace_len = ft_strlen(replace);
	len = ft_strlen(str) - find_len + replace_len;
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	ft_strlcpy(new, str, pos - str + 1);
	ft_strlcat(new, replace, len + 1);
	ft_strlcat(new, pos + find_len, len + 1);
	return (new);
}
