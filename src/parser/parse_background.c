/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_background.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:36:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:14:42 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

t_ast	*parse_background(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_sequence(cur);
	if (!left)
		return (NULL);
	while (*cur && (*cur)->type == T_BG)
	{
		next_token(cur);
		if (!*cur || (*cur)->type == T_SEMI || (*cur)->type == T_PIPE
			|| (*cur)->type == T_AND || (*cur)->type == T_OR
			|| (*cur)->type == T_RPAREN)
		{
			// No hay más comandos, se permite background sin "right"
			node = ast_new_binary(N_BACKGROUND, left, NULL);
			if (!node)
			{
				free_ast(left);
				return (NULL);
			}
			left = node;
			break ;
		}
		right = parse_sequence(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = ast_new_binary(N_BACKGROUND, left, right);
		if (!node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		left = node;
	}
	return (left);
}
