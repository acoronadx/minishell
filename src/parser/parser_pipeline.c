/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:38 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/13 20:02:05 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** next_token:
** Avanza el puntero actual de token al siguiente elemento de la lista.
** Se usa para recorrer la lista de tokens generada por el lexer.
*/
void	next_token(t_token **cur)
{
	if (*cur)
		*cur = (*cur)->next;
}

// pipeline: comando | comando
t_ast	*parse_pipeline(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_simple_command(cur);
	while (*cur && (*cur)->type == T_PIPE)
	{
		next_token(cur);
		right = parse_simple_command(cur);
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = N_PIPE;
		node->left = left;
		node->right = right;
		node->argv = NULL;
		node->filename = NULL;
		node->redir_type = 0;
		left = node;
	}
	return (left);
}
