/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 00:41:28 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 00:55:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errors.h"

void	add_cmd_to_list(t_cmd **head, t_cmd **last, t_cmd *cmd)
{
	if (!*head)
		*head = cmd;
	else
		(*last)->next = cmd;
	*last = cmd;
}

t_cmd	*check_pipe_syntax(t_token *tokens, t_cmd *head)
{
	const char	*unexpected;

	unexpected = "newline";
	if (tokens->next && tokens->next->type == T_PIPE)
		unexpected = "|";
	if (!tokens->next || tokens->next->type == T_PIPE)
	{
		free_cmds(head);
		print_syntax_error(unexpected);
		return (NULL);
	}
	return (NULL);
}
