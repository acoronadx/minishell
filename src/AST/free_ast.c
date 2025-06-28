/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:30:28 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 03:56:18 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"
#include "parser.h"
#include "lexer.h"

// Libera solo un nodo, sin liberar hijos ni cadenas externas
void free_ast(t_ast *node)
{
    if (!node)
        return;

    if (node->type == N_COMMAND)
    {
        if (node->cmd.argv)
            ft_free_strtab(node->cmd.argv); // función para liberar char** argv
        // redirections NO se liberan aquí, porque pueden ser compartidas o liberadas en free_ast_recursive
    }
    else if (node->type == N_REDIR)
    {
        if (node->redir.filename)
            free(node->redir.filename);
        if (node->redir.delimiter)
            free(node->redir.delimiter);
    }
    // No libera punteros a hijos (bin, subshell), solo el nodo actual
    free(node);
}

// Libera el árbol entero recursivamente (incluye hijos y redirecciones)
void free_ast_recursive(t_ast *node)
{
    if (!node)
        return;
    if (node->type == N_COMMAND)
    {
        free_ast_recursive(node->cmd.redirections);
        if (node->cmd.argv)
            ft_free_strtab(node->cmd.argv);
    }
    else if (node->type == N_REDIR)
    {
        if (node->redir.filename)
            free(node->redir.filename);
        if (node->redir.delimiter)
            free(node->redir.delimiter);
    }
    else if (node->type == N_SUBSHELL)
    {
        free_ast_recursive(node->subshell.child);
        free_ast_recursive(node->subshell.redirections);
    }
    else // para nodos binarios (PIPE, SEQUENCE, AND, OR, BACKGROUND)
    {
        free_ast_recursive(node->bin.left);
        free_ast_recursive(node->bin.right);
    }
    free(node);
}
