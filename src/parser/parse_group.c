/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 00:59:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 03:37:23 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"
#include "lexer.h"

t_ast	*parse_group(t_token **cur)
{
	t_ast	*sub_ast;
	t_ast	*node;

	next_token(cur);
	if (!*cur)
	{
		ft_dprintf(2,
			"minishell: syntax error: unexpected end of input after '('\n");
		return (NULL);
	}
	sub_ast = parse_background(cur);
	if (!sub_ast)
		return (NULL);
	if (!(*cur) || (*cur)->type != T_RPAREN)
	{
		ft_dprintf(2, "minishell: syntax error: missing ')'\n");
		free_ast(sub_ast);
		return (NULL);
	}
	next_token(cur);
	node = ast_new_subshell(sub_ast, NULL);
	if (!node)
	{
		free_ast(sub_ast);
		return (NULL);
	}
	return (node);
}
