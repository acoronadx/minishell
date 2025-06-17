/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_background.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:36:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:58:04 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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
/*
** background := sequence ( '&' sequence )*
*/
t_ast	*parse_background(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_sequence(cur);
	while (*cur && (*cur)->type == T_BG)
	{
		next_token(cur);
		right = parse_sequence(cur);
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = N_BACKGROUND;
		node->left = left;
		node->right = right;
		node->argv = NULL;
		node->filename = NULL;
		node->redir_type = 0;
		left = node;
	}
	return (left);
}

/*
** sequence := and_or ( ';' and_or )*
*/
t_ast	*parse_sequence(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_and_or(cur);
	while (*cur && (*cur)->type == T_SEMI)
	{
		next_token(cur);
		right = parse_and_or(cur);
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = N_SEQUENCE;
		node->left = left;
		node->right = right;
		node->argv = NULL;
		node->filename = NULL;
		node->redir_type = 0;
		left = node;
	}
	return (left);
}
