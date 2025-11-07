/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 01:03:45 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:39:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifica que haya WORD tras el operador y que el tipo sea válido */
static int      expect_word_after_op(t_token *file_tok, t_redir_type rtype)
{
        if (!file_tok || file_tok->type != T_WORD || rtype == REDIR_INVALID)
        {
                ft_dprintf(2,
                "minishell: syntax error near unexpected token '%s'\n",
                file_tok ? file_tok->value : "newline");
                return (0);
        }
        return (1);
}

/* Inserta al final de la lista enlazada de redirecciones */
static void     append_redir(t_ast **head, t_ast **tail, t_ast *node)
{
        if (!*head)
        {
                *head = node;
                *tail = node;
        }
        else
        {
                (*tail)->bin.right = node;
                *tail = node;
        }
}

/* Parsea una redirección: OP WORD -> crea nodo y lo añade a la lista */
static int      parse_one_redirection(t_token **cur, t_ast **head, t_ast **tail)
{
        t_redir_type    rtype;
        t_token         *file_tok;
        t_ast           *new_redir;

        rtype = token_type_to_redir_type((*cur)->type);
        next_token(cur);
        file_tok = *cur;
        if (!expect_word_after_op(file_tok, rtype))
                return (0);
        if (rtype == REDIR_DUP_OUT || rtype == REDIR_DUP_IN)
                new_redir = build_dup_node(file_tok, rtype);
        else if (rtype == REDIR_HEREDOC)
                new_redir = build_heredoc_node(file_tok);
        else
                new_redir = build_file_node(file_tok, rtype);
        if (!new_redir)
                return (0);
        next_token(cur);
        append_redir(head, tail, new_redir);
        return (1);
}

int     parse_redirections(t_token **cur, t_ast **head, t_ast **tail)
{
        *head = NULL;
        *tail = NULL;
        while (*cur && is_redirection(*cur))
        {
                if (!parse_one_redirection(cur, head, tail))
                        return (0);
        }
        return (1);
}
