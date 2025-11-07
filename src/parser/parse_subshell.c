/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 00:59:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:03:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Avanza después de '(' y valida que no acabe el input inmediatamente. */
static int	consume_open_paren(t_token **cur)
{
	next_token(cur);
	if (!*cur)
	{
		ft_dprintf(2,
			"minishell: syntax error: unexpected end of input after '('\n");
		return (0);
	}
	return (1);
}

/* Exige ')' como siguiente token, lo consume o informa error y libera sub_ast. */
static int	check_and_consume_rparen(t_token **cur, t_ast *sub_ast)
{
	if (!(*cur) || (*cur)->type != T_RPAREN)
	{
		ft_dprintf(2, "minishell: syntax error: missing ')'\n");
		free_ast(sub_ast);
		return (0);
	}
	next_token(cur);
	return (1);
}

/* Crea el nodo subshell o libera en caso de fallo. */
static t_ast	*make_subshell_node(t_ast *sub_ast)
{
	t_ast	*node;

	node = ast_new_subshell(sub_ast, NULL);
	if (!node)
	{
		free_ast(sub_ast);
		return (NULL);
	}
	return (node);
}

/*
** parse_subshell:
**  - Se asume que el token actual es '(' (ya reconocido por el caller).
**  - Avanza tras '(' y parsea el cuerpo con parse_background().
**  - Exige ')' y la consume.
**  - Devuelve un nodo AST de tipo subshell.
*/
t_ast	*parse_subshell(t_token **cur)
{
	t_ast	*sub_ast;

	if (!consume_open_paren(cur))
		return (NULL);
	sub_ast = parse_background(cur);
	if (!sub_ast)
		return (NULL);
	if (!check_and_consume_rparen(cur, sub_ast))
		return (NULL);
	return (make_subshell_node(sub_ast));
}
