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

static char	*create_replacement(char *str, char *pos, char *find, char *replace)
{
	char	*new;
	size_t	len;
	size_t	prefix_len;

	len = ft_strlen(str) - ft_strlen(find) + ft_strlen(replace);
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	prefix_len = pos - str;
	ft_strlcpy(new, str, prefix_len + 1);
	ft_strlcat(new, replace, len + 1);
	ft_strlcat(new, pos + ft_strlen(find), len + 1);
	return (new);
}

char	*ft_strreplace(char *str, char *find, char *replace)
{
	char	*pos;
	size_t	find_len;

	if (!str || !find || !replace)
		return (NULL);
	find_len = ft_strlen(find);
	if (find_len == 0)
		return (ft_strdup(str));
	pos = ft_strnstr(str, find, ft_strlen(str));
	if (!pos)
		return (ft_strdup(str));
	return (create_replacement(str, pos, find, replace));
}
