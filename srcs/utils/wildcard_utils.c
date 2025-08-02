/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*str)
		{
			if (match_pattern(str, pattern + 1))
				return (1);
			str++;
		}
		return (match_pattern(str, pattern + 1));
	}
	if (*str && (*str == *pattern || *pattern == '?'))
		return (match_pattern(str + 1, pattern + 1));
	return (0);
}

int	count_matches(char *dir_path, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && match_pattern(entry->d_name, pattern))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

char	**allocate_result_array(int count)
{
	char	**result;

	if (count == 0)
		return (NULL);
	result = malloc(sizeof(char *) * (count + 1));
	return (result);
}

char	*create_full_path(char *dir_path, char *filename)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir_path, "/");
	full_path = ft_strjoin(temp, filename);
	free(temp);
	return (full_path);
}

char	**handle_no_wildcard(char *str)
{
	char	**result;

	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = ft_strdup(str);
	result[1] = NULL;
	return (result);
}
