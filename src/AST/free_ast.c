/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 02:30:28 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 15:43:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Libera solo un nodo (ya la tienes) */
void    free_ast(t_ast *node)
{
    if (!node)
        return ;
    if (node->type == N_COMMAND)
    {
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
    free(node);
}

/* Árbol entero recursivo (dispatcher) */
void    free_ast_recursive(t_ast *node)
{
    if (!node)
        return ;
    if (node->type == N_COMMAND)
        free_ast_cmd(node);
    else if (node->type == N_REDIR)
        free_ast_redir(node);
    else
        free_ast_sub_or_bin(node);
    free(node);
}
