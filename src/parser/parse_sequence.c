/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:23:59 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:14:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"

t_ast	*parse_sequence(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_and_or(cur);
	if (!left)
		return (NULL);

	while (*cur && (*cur)->type == T_SEMI)
	{
		next_token(cur);
		if (!*cur)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token 'newline'\n");
			free_ast(left);
			return (NULL);
		}
		right = parse_and_or(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = ast_new_binary(N_SEQUENCE, left, right);
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
