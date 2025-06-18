/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:26:10 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:26:11 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new;
	char	*equal;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (!equal)
			continue ;
		new = malloc(sizeof(t_env));
		if (!new)
			return (free_env(env), NULL);
		new->key = ft_substr(envp[i], 0, equal - envp[i]);
		new->value = ft_strdup(equal + 1);
		new->next = env;
		env = new;
		i++;
	}
	return (env);
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env, char *key, char *value)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	current = malloc(sizeof(t_env));
	if (!current)
		return ;
	current->key = ft_strdup(key);
	current->value = ft_strdup(value);
	current->next = *env;
	*env = current;
}

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*str;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	str = malloc(len1 + len2 + len3 + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcpy(str + len1, s2, len2 + 1);
	ft_strlcpy(str + len1 + len2, s3, len3 + 1);
	return (str);
}
