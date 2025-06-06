/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:28:05 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:29:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	free_tokens(t_token *token_list)
{
	t_token	*tmp;

	while (token_list)
	{
		tmp = token_list->next;
		free(token_list->value);
		free(token_list);
		token_list = tmp;
	}
}

void	cleanup_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->line)
		free(shell->line);
	free_tokens(shell->tokens);
	cleanup_env_list(shell->env_list);
}
