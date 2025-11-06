/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:35:01 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:20:24 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*join_key_value(const char *key, const char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	if (!value)
		res = ft_strdup(tmp); /* "KEY=" */
	else
		res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

static void	free_strv(char **v)
{
	int	i;

	if (!v)
		return ;
	i = 0;
	while (v[i])
	{
		free(v[i]);
		i++;
	}
	free(v);
}

char	**env_to_array(t_env *env)
{
	char	**env_array;
	int		size;
	int		i;

	size = env_count(env);
	env_array = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported)
		{
			env_array[i] = join_key_value(env->key, env->value);
			if (!env_array[i])
			{
				free_strv(env_array);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
