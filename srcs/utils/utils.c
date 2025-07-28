/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:49:57 by rsommer           #+#    #+#             */
/*   Updated: 2025/07/22 10:49:59 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strreplace(char *str, char *old_str, char *new_str)
{
	char	*result;
	char	*pos;
	int		old_len;
	int		new_len;
	int		result_len;

	if (!str || !old_str || !new_str)
		return (NULL);
	pos = ft_strnstr(str, old_str, ft_strlen(str));
	if (!pos)
		return (ft_strdup(str));
	old_len = ft_strlen(old_str);
	new_len = ft_strlen(new_str);
	result_len = ft_strlen(str) - old_len + new_len;
	result = malloc(result_len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, pos - str + 1);
	ft_strlcat(result, new_str, result_len + 1);
	ft_strlcat(result, pos + old_len, result_len + 1);
	return (result);
}
