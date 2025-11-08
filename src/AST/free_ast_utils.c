/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:42:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 16:54:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_redir_list(t_ast *r)
{
    t_ast *n;

    n = NULL;
    while (r)
    {
        n = r->next;
        free_ast(r); /* N_REDIR: free de filename/delimiter y nodo */
        r = n;
    }
}

void free_ast_cmd(t_ast *node)
{
    if (!node)
        return ;
    free_redir_list(node->cmd.redirections);
    if (node->cmd.argv)
        ft_free_strtab(node->cmd.argv);
}

void free_ast_redir(t_ast *node)
{
    if (!node)
        return ;
    if (node->redir.filename)
        free(node->redir.filename);
    if (node->redir.delimiter)
        free(node->redir.delimiter);
}

void free_ast_sub_or_bin(t_ast *node)
{
    if (!node)
        return ;
    if (node->type == N_SUBSHELL)
    {
        free_ast_recursive(node->subshell.child);
        free_redir_list(node->subshell.redirections);
        return ;
    }
    free_ast_recursive(node->bin.left);
    free_ast_recursive(node->bin.right);
}
