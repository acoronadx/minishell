/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:36:06 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/13 18:41:30 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Función principal del parser: construye el AST a partir de la lista de tokens.
** Devuelve la raíz del AST o NULL en caso de error.
*/
t_ast	*build_ast(t_token *tokens)
{
	t_token	*cur;
	t_ast	*tree;

	cur = tokens;
	tree = parse_background(&cur);
	if (cur && cur->type != T_UNKNOWN)
	{
		free_ast(tree);
		return (NULL);
	}
	return (tree);
}
