/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_or.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:03 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 03:02:13 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"

t_ast	*parse_and_or(t_token **cur)
{
	t_ast		*left;
	t_ast		*right;
	t_ast		*node;
	t_node_type	node_type;

	left = parse_pipeline(cur);
	if (!left)
		return (NULL);

	while (*cur && ((*cur)->type == T_AND || (*cur)->type == T_OR))
	{
		if ((*cur)->type == T_AND)
			node_type = N_AND;
		else
			node_type = N_OR;

		next_token(cur);
		if (!*cur)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token 'newline'\n");
			free_ast(left);
			return (NULL);
		}
		right = parse_pipeline(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = ast_new_binary(node_type, left, right);
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
