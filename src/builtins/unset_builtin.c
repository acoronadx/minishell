/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:27:13 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 12:27:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

// ----------- UNSET -----------
int	run_unset(char **argv, t_shell *shell)
{
	int		i;
	t_env	*prev;
	t_env	*cur;

	i = 1;
	while (argv[i])
	{
		prev = NULL;
		cur = shell->env;
		while (cur)
		{
			if (ft_strcmp(cur->key, argv[i]) == 0)
			{
				if (prev)
					prev->next = cur->next;
				else
					shell->env = cur->next;
				free(cur->key);
				free(cur->value);
				free(cur);
				break ;
			}
			prev = cur;
			cur = cur->next;
		}
		i++;
	}
	return (0);
}
