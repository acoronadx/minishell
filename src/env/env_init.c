/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:31:13 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:33:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_shlvl(t_env **env_list)
{
	t_env	*shlvl;
	int		lvl;
	char	*new_value;

	shlvl = get_env_node(*env_list, "SHLVL");
	if (shlvl && shlvl->value)
	{
		lvl = ft_atoi(shlvl->value);
		free(shlvl->value);
		shlvl->value = ft_itoa(lvl + 1);
	}
	else if (!shlvl)
	{
		t_env *new = env_create("SHLVL", "1", 1);
		if (!new)
			return ;
		env_add_back(env_list, new);
	}
}

static void	add_env_var(t_env **env_list, char **key_value)
{
	t_env	*new;

	if (!get_env_node(*env_list, key_value[0]))
	{
		new = env_create(key_value[0],
				key_value[1] ? key_value[1] : NULL, 1);
		if (!new)
			return ;
		env_add_back(env_list, new);
	}
}

t_env	*init_env_list(char **envp)
{
	t_env	*env_list = NULL;
	char	**key_value;
	int		i;

	i = 0;
	while (envp[i])
	{
		key_value = ft_split_once(envp[i], '=');
		if (!key_value || !key_value[0])
		{
			ft_free_split(key_value);
			i++;
			continue ;
		}
		add_env_var(&env_list, key_value);
		ft_free_split(key_value);
		i++;
	}
	handle_shlvl(&env_list);
	return (env_list);
}
