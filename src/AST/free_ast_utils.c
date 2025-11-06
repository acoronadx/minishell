/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:42:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 15:43:03 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* libera N_COMMAND: redirs recursivas + argv */
void    free_ast_cmd(t_ast *node)
{
    if (!node)
        return ;
    free_ast_recursive(node->cmd.redirections);
    if (node->cmd.argv)
        ft_free_strtab(node->cmd.argv);
}

/* libera N_REDIR: filename + delimiter */
void    free_ast_redir(t_ast *node)
{
    if (!node)
        return ;
    if (node->redir.filename)
        free(node->redir.filename);
    if (node->redir.delimiter)
        free(node->redir.delimiter);
}

/* libera N_SUBSHELL o binario (PIPE/AND/OR/SEQUENCE/BACKGROUND) */
void    free_ast_sub_or_bin(t_ast *node)
{
    if (!node)
        return ;
    if (node->type == N_SUBSHELL)
    {
        free_ast_recursive(node->subshell.child);
        free_ast_recursive(node->subshell.redirections);
        return ;
    }
    /* binario */
    free_ast_recursive(node->bin.left);
    free_ast_recursive(node->bin.right);
}
