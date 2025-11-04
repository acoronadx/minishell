/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:53:52 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:20:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

char	*get_env_value(t_shell *shell, const char *key)
{
	t_env	*env;

	if (!shell || !key)
		return (NULL);
	env = shell->env;
	while (env)
	{
		if (env->key && !ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
