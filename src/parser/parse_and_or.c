/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:03 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 15:07:41 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Si hay &&/||, fija type y consume; si falta el token siguiente -> error */
static int  consume_andor_or_report(t_token **cur, t_ast *left,
                                    t_node_type *out_type)
{
    if (!*cur)
        return (0);
    if ((*cur)->type == T_AND)
        *out_type = N_AND;
    else if ((*cur)->type == T_OR)
        *out_type = N_OR;
    else
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

/* Parsea la derecha como pipeline o limpia left en caso de error */
static t_ast        *parse_right_pipe_or_cleanup(t_token **cur, t_ast *left)
{
        t_ast *right;

        right = parse_pipeline(cur);
        if (!right)
        {
                free_ast(left);
                return (NULL);
        }
        return (right);
}

/* Construye nodo AND/OR o limpia ambas ramas si falla */
static t_ast        *make_andor_node_or_cleanup(t_node_type type,
                                                t_ast *left, t_ast *right)
{
        t_ast *node;

        node = ast_new_binary(type, left, right);
        if (!node)
        {
                free_ast(left);
                free_ast(right);
                return (NULL);
        }
        return (node);
}

/* Itera: left && right  ||  left || right ... */
static t_ast *andor_tail(t_token **cur, t_ast *left)
{
	t_ast        *right;
	t_ast        *node;
	t_node_type   type;
	int           r;

	while (1)
	{
		r = consume_andor_or_report(cur, left, &type);
		if (r <= 0)
		{
			if (r == 0)
				return (left);
			return (NULL);
		}
		right = parse_right_pipe_or_cleanup(cur, left);
		if (!right)
			return (NULL);
		node = make_andor_node_or_cleanup(type, left, right);
		if (!node)
			return (NULL);
		left = node;
	}
}


t_ast   *parse_and_or(t_token **cur)
{
        t_ast *left;

        left = parse_pipeline(cur);
        if (!left)
                return (NULL);
        return (andor_tail(cur, left));
}
