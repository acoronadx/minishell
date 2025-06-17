/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_or.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:03 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:08:23 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"

/*
** and_or := pipeline ( ( '&&' | '||' ) pipeline )*
*/
t_ast	*parse_and_or(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	int		type;

	left = parse_pipeline(cur);
	while (*cur && ((*cur)->type == T_AND || (*cur)->type == T_OR))
	{
		type = (*cur)->type == T_AND ? N_AND : N_OR;
		next_token(cur);
		right = parse_pipeline(cur);
		node = malloc(sizeof(t_ast));
		if (!node)
			return (NULL);
		node->type = type;
		node->left = left;
		node->right = right;
		node->argv = NULL;
		node->filename = NULL;
		node->redir_type = 0;
		left = node;
	}
	return (left);
}
