/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:23:59 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:51:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Si hay ';' lo consume; si falta el token siguiente, reporta y libera left */
static int      consume_semicolon_or_report(t_token **cur, t_ast *left)
{
        if (!(*cur) || (*cur)->type != T_SEMI)
                return (0);
        next_token(cur);
        if (!*cur)
        {
                ft_dprintf(2,
                "minishell: syntax error near unexpected token 'newline'\n");
                free_ast(left);
                return (-1);
        }
        return (1);
}

/* Parsea el lado derecho o, si falla, libera left y devuelve NULL */
static t_ast    *parse_right_or_cleanup(t_token **cur, t_ast *left)
{
        t_ast   *right;

        right = parse_and_or(cur);
        if (!right)
        {
                free_ast(left);
                return (NULL);
        }
        return (right);
}

/* Construye nodo N_SEQUENCE o limpia ambos lados si falla */
static t_ast    *make_seq_node_or_cleanup(t_ast *left, t_ast *right)
{
        t_ast   *node;

        node = ast_new_binary(N_SEQUENCE, left, right);
        if (!node)
        {
                free_ast(left);
                free_ast(right);
                return (NULL);
        }
        return (node);
}

/* Bucle de secuencia: left ; right ; right2 ; ... */
static t_ast    *sequence_tail(t_token **cur, t_ast *left)
{
        t_ast   *right;
        t_ast   *node;
        int     r;

        while (1)
        {
                r = consume_semicolon_or_report(cur, left);
				if (r <= 0)
				{
						if (r == 0)
								return (left);
						return (NULL);
				}
                right = parse_right_or_cleanup(cur, left);
                if (!right)
                        return (NULL);
                node = make_seq_node_or_cleanup(left, right);
                if (!node)
                        return (NULL);
                left = node;
        }
}

t_ast   *parse_sequence(t_token **cur)
{
        t_ast   *left;

        left = parse_and_or(cur);
        if (!left)
                return (NULL);
        return (sequence_tail(cur, left));
}
