/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 07:31:30 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:36:39 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type token_type_to_redir_type(t_token_type type)
{
    if (type == T_REDIR_IN)
		return REDIR_IN;
    if (type == T_REDIR_OUT)
		return REDIR_OUT;
    if (type == T_APPEND)
		return REDIR_APPEND;
    if (type == T_HEREDOC)
		return REDIR_HEREDOC;
    if (type == T_REDIR_ERR)
		return REDIR_ERR;
    if (type == T_APPEND_ERR)
		return REDIR_APPEND_ERR;
    if (type == T_REDIR_ALL)
		return REDIR_ALL;
    if (type == T_APPEND_ALL)
		return REDIR_APPEND_ALL;
    if (type == T_FORCE_OUT)
		return REDIR_FORCE;
    if (type == T_DUP_IN)
		return REDIR_DUP_IN;
    if (type == T_DUP_OUT)
		return REDIR_DUP_OUT;
    return REDIR_INVALID;
}

int is_redirection(t_token *tok)
{
    if (!tok) return 0;
    return (tok->type == T_REDIR_IN || tok->type == T_REDIR_OUT
        || tok->type == T_APPEND   || tok->type == T_HEREDOC
        || tok->type == T_REDIR_ERR   
        || tok->type == T_APPEND_ERR  
        || tok->type == T_REDIR_ALL || tok->type == T_APPEND_ALL
        || tok->type == T_FORCE_OUT || tok->type == T_DUP_IN
        || tok->type == T_DUP_OUT);
}

/* >&/ <& : duplicación de FD (requiere un entero positivo) */
t_ast    *build_dup_node(t_token *file_tok, t_redir_type rtype)
{
        int     fd;

        if (!ft_isposfdstr(file_tok->value))
        {
                ft_dprintf(2,
                "minishell: syntax error near unexpected token '%s'\n",
                file_tok->value);
                return (NULL);
        }
        fd = ft_atoi(file_tok->value);
        return (ast_new_redir(NULL, NULL, rtype, fd));
}

/* << : here-doc (guardar delimitador y flag de quoted) */
t_ast    *build_heredoc_node(t_token *file_tok)
{
        t_ast   *node;

        node = ast_new_redir(NULL, ft_strdup(file_tok->value),
                             REDIR_HEREDOC, -1);
        if (!node)
                return (NULL);
        node->redir.heredoc_quoted = (file_tok->quoted != NO_QUOTE);
        return (node);
}

/* >, >>, < : redirección con nombre de archivo */
t_ast    *build_file_node(t_token *file_tok, t_redir_type rtype)
{
        return (ast_new_redir(ft_strdup(file_tok->value), NULL, rtype, -1));
}
