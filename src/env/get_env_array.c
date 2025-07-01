/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:35:01 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/29 15:55:08 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

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
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
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
	while (env && i < size)
	{
		if (env->exported)
		{
			env_array[i] = join_key_value(env->key, env->value);
			if (!env_array[i])
			{
				ft_free_strtab(env_array);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
