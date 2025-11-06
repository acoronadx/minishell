/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:28:27 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 17:06:16 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Based on POSIX definition of valid environment variable names
// “Environment variable names consist solely of uppercase letters, digits,
// and the underscore (‘_’). The first character shall not be a digit.”
static int	is_valid_env_name(const char *s)
{
	int				i;	
	unsigned char	c;

	if (!s || !*s)
		return (0);
	c = (unsigned char)s[0];
	if (!(c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
		return (0);
	i = 1;
	while (s[i])
	{
		c = (unsigned char)s[i];
		if (!(c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
				|| (c >= '0' && c <= '9')))
			return (0);
		i++;
	}
	return (1);
}

t_env	*env_create(char *key, char *value, int exported)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (!new->key)
		return (free(new), NULL);
	new->value = (value) ? ft_strdup(value) : NULL;
	if (value && !new->value)
		return (free(new->key), free(new), NULL);
	new->exported = exported;
	new->next = NULL;
	return (new);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!env || !new)
		return ;
	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static int	add_exported_from_str(char *str, t_env **env)
{
	char	*sep;
	char	*key;
	t_env	*new;

	sep = ft_strchr(str, '=');
	if (!sep)
		return (1); /* ignorar entradas sin '=' */
	key = ft_substr(str, 0, sep - str);
	if (!key)
		return (0);
	if (!is_valid_env_name(key))
		return (free(key), 1); /* ignorar nombres inválidos */
	new = env_create(key, sep + 1, 1);
	free(key);
	if (!new)
		return (0);
	env_add_back(env, new);
	return (1);
}

t_env	*init_env_list(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (!add_exported_from_str(envp[i], &env))
		{
			free_env_list(env);
			return (NULL);
		}
		i++;
	}
	return (env);
}
