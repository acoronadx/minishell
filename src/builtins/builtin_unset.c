/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:40:06 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:40:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	env_remove_key(t_env **env, const char *key)
{
	t_env	*prev = NULL;
	t_env	*curr = *env;
	t_env	*tmp;

	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			tmp = curr;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_dprintf(2, "unset: `%s`: not a valid identifier\n", args[i]);
			exit_code = 1;
		}
		else
			env_remove_key(&shell->env_list, args[i]);
		i++;
	}
	return (exit_code);
}
