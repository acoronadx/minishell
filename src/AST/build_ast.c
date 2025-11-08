/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:36:06 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 17:21:23 by acoronad         ###   ########.fr       */
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
	tree = parse_background(&cur); // Llama a la función de más alta precedencia
	if (!tree)
		return (NULL);
	// Si quedan tokens sin procesar,
	// es un error de sintaxis que las funciones de abajo
	// no capturaron porque no formaban parte de ninguna regla sintáctica esperada.
	// Esto es muy importante para detectar cosas como "ls
	//	&& ;" o "echo hello world extra_stuff"
	// donde "extra_stuff" no es parte de la sintaxis válida después de un comando simple.
	if (cur != NULL)
		// <-- Esta condición es correcta para detectar tokens no consumidos
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token '%s'\n",
			cur->value);
		free_ast(tree);
		return (NULL);
	}
	return (tree);
}
