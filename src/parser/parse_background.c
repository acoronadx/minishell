/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_background.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:36:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:21:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Paramos si no hay token o si viene ; | && || ) */
static int      is_bg_stop(const t_token *tok)
{
        if (!tok)
                return (1);
        return (tok->type == T_SEMI || tok->type == T_PIPE
                || tok->type == T_AND || tok->type == T_OR
                || tok->type == T_RPAREN);
}

/* Construye N_BACKGROUND o limpia y falla */
static t_ast    *make_bg_node_or_cleanup(t_ast *left, t_ast *right)
{
        t_ast   *node;

        node = ast_new_binary(N_BACKGROUND, left, right);
        if (!node)
        {
                free_ast(left);
                if (right)
                        free_ast(right);
                return (NULL);
        }
        return (node);
}

/* Parsea la parte derecha o limpia left si falla */
static t_ast    *parse_right_sequence_or_cleanup(t_token **cur, t_ast *left)
{
        t_ast   *right;

        right = parse_sequence(cur);
        if (!right)
        {
                free_ast(left);
                return (NULL);
        }
        return (right);
}

/* Bucle: left & [right]?  (permite & final sin comando a la derecha) */
static t_ast    *background_tail(t_token **cur, t_ast *left)
{
        t_ast   *right;
        t_ast   *node;

        while (*cur && (*cur)->type == T_BG)
        {
                next_token(cur);
                if (is_bg_stop(*cur))
                {
                        node = make_bg_node_or_cleanup(left, NULL);
                        if (!node)
                                return (NULL);
                        return (node); /* '&' final: cerramos la cadena */
                }
                right = parse_right_sequence_or_cleanup(cur, left);
                if (!right)
                        return (NULL);
                node = make_bg_node_or_cleanup(left, right);
                if (!node)
                        return (NULL);
                left = node;
        }
        return (left);
}

t_ast   *parse_background(t_token **cur)
{
        t_ast   *left;

        left = parse_sequence(cur);
        if (!left)
                return (NULL);
        return (background_tail(cur, left));
}
