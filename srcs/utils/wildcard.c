/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**read_directory_entries(char *dir_path, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;
	int				i;

	count = count_matches(dir_path, pattern);
	result = allocate_result_array(count);
	if (!result)
		return (NULL);
	dir = opendir(dir_path);
	if (!dir)
		return (free(result), NULL);
	i = 0;
	entry = readdir(dir);
	while (entry != NULL && i < count)
	{
		if (entry->d_name[0] != '.' && match_pattern(entry->d_name, pattern))
			result[i++] = create_full_path(dir_path, entry->d_name);
		entry = readdir(dir);
	}
	result[i] = NULL;
	closedir(dir);
	return (result);
}

char	**expand_wildcards(char *str)
{
	char	**result;
	char	*last_slash;
	char	*dir_path;
	char	*pattern;

	if (!ft_strchr(str, '*'))
		return (handle_no_wildcard(str));
	last_slash = ft_strrchr(str, '/');
	if (last_slash)
	{
		dir_path = ft_substr(str, 0, last_slash - str);
		pattern = ft_strdup(last_slash + 1);
	}
	else
	{
		dir_path = ft_strdup(".");
		pattern = ft_strdup(str);
	}
	result = read_directory_entries(dir_path, pattern);
	free(dir_path);
	free(pattern);
	return (result);
}

void	free_wildcard_results(char **results)
{
	int	i;

	if (!results)
		return ;
	i = 0;
	while (results[i])
	{
		free(results[i]);
		i++;
	}
	free(results);
}
