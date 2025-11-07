/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:05:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:10:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* [redirs] iniciales */
static int      parse_leading_redirs(t_token **cur, t_ast **head, t_ast **tail)
{
        if (!parse_redirections(cur, head, tail))
                return (0);
        return (1);
}

/* Caso bash: permitir solo redirecciones sin argv; si no hay redirs -> error */
static t_ast    *handle_no_argv_case(t_ast *redir_head)
{
        if (redir_head)
                return (ast_new_command(NULL, redir_head));
        ft_dprintf(2, "minishell: syntax error: empty command\n");
        return (NULL);
}

/* [redirs] finales; en error, limpiar argv y redirs */
static int      parse_trailing_redirs_ok(t_token **cur, t_ast **head,
                                         t_ast **tail, char **argv)
{
        if (!parse_redirections(cur, head, tail))
        {
                ft_free_strtab(argv);
                free_ast(*head);
                return (0);
        }
        return (1);
}

/* Construcción del nodo comando o limpieza si falla */
static t_ast    *create_cmd_or_cleanup(char **argv, t_ast *redir_head)
{
        t_ast   *cmd_node;

        cmd_node = ast_new_command(argv, redir_head);
        if (!cmd_node)
        {
                ft_free_strtab(argv);
                free_ast(redir_head);
                return (NULL);
        }
        return (cmd_node);
}

t_ast    *handle_plain_command(t_token **cur)
{
        t_ast   *redir_head;
        t_ast   *redir_tail;
        char    **argv;

        redir_head = NULL;
        redir_tail = NULL;
        if (!parse_leading_redirs(cur, &redir_head, &redir_tail))
                return (NULL);
        argv = parse_arguments(cur);
        if (!argv)
                return (handle_no_argv_case(redir_head));
        if (!parse_trailing_redirs_ok(cur, &redir_head, &redir_tail, argv))
                return (NULL);
        return (create_cmd_or_cleanup(argv, redir_head));
}
