/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:38 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:51:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Si hay '|' lo consume; si luego no hay token, error y free(left) */
static int      consume_pipe_or_report(t_token **cur, t_ast *left)
{
        if (!(*cur) || (*cur)->type != T_PIPE)
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

/* Parsea el comando derecho o libera left en caso de error */
static t_ast    *parse_right_cmd_or_cleanup(t_token **cur, t_ast *left)
{
        t_ast   *right;

        right = parse_command_and_redirections(cur);
        if (!right)
        {
                free_ast(left);
                return (NULL);
        }
        return (right);
}

/* Construye nodo N_PIPE o limpia ambos lados si falla */
static t_ast    *make_pipe_node_or_cleanup(t_ast *left, t_ast *right)
{
        t_ast   *node;

        node = ast_new_binary(N_PIPE, left, right);
        if (!node)
        {
                free_ast(left);
                free_ast(right);
                return (NULL);
        }
        return (node);
}

/* Bucle: left | right | right2 ... */
static t_ast    *pipeline_tail(t_token **cur, t_ast *left)
{
        t_ast   *right;
        t_ast   *node;
        int     r;

        while (1)
        {
                r = consume_pipe_or_report(cur, left);
				if (r <= 0)
				{
						if (r == 0)
								return (left);
						return (NULL);
				}
                right = parse_right_cmd_or_cleanup(cur, left);
                if (!right)
                        return (NULL);
                node = make_pipe_node_or_cleanup(left, right);
                if (!node)
                        return (NULL);
                left = node;
        }
}

t_ast   *parse_pipeline(t_token **cur)
{
        t_ast   *left;

        left = parse_command_and_redirections(cur);
        if (!left)
                return (NULL);
        return (pipeline_tail(cur, left));
}