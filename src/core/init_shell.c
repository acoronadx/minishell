/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:46:26 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 19:46:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_create(char *key, char *value, int exported)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = value ? ft_strdup(value) : NULL;
	new->exported = exported;
	new->next = NULL;
	return (new);
}

void	env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	init_env_list(t_shell *shell, char **envp)
{
	int		i;
	char	*sep;
	t_env	*new;

	shell->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			new = env_create(
				ft_substr(envp[i], 0, sep - envp[i]),
				sep + 1, // ya es puntero al value
				1
			);
			if (!new)
				ft_print_exit_init_error("malloc", shell);
			env_add_back(&shell->env_list, new);
		}
		i++;
	}
}

void	init_shell(t_shell *shell, char **envp)
{
	init_env_list(shell, envp);
	shell->line = NULL;
	shell->tokens = NULL;
	shell->exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1)
		ft_print_exit_init_error("failed to duplicate stdin/stdout", shell);
}
