/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:51:59 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:13:39 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Parses the raw input line into an abstract syntax tree (AST).
** Returns a pointer to the root node or NULL on error.
*/
t_ast	*parse_line(char *line)
{
	t_token	*tokens;
	t_ast	*root;

	tokens = lexer(line);
	if (!tokens)
		return (NULL);
	root = build_ast(tokens);
	free_tokens(tokens);
	return (root);
}
