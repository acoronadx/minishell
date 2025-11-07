/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:39:45 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 15:47:32 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int	is_lparen(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == T_LPAREN);
}


/* ( ... ) [redirs] */
static t_ast    *handle_subshell_branch(t_token **cur)
{
    t_ast   *cmd_node;
    t_ast   *redir_head;
    t_ast   *redir_tail;

    redir_head = NULL;
    redir_tail = NULL;
    cmd_node = parse_subshell(cur);
    if (!cmd_node)
            return (NULL);
    if (!parse_redirections(cur, &redir_head, &redir_tail))
    {
            free_ast(cmd_node);
            return (NULL);
    }
    cmd_node->subshell.redirections = redir_head;
    return (cmd_node);
}

/*
** parse_simple_command:
**  - Si ve '(' delega en rama de subshell.
**  - Si no, parsea redirecciones iniciales, argv y redirecciones finales.
*/
t_ast   *parse_simple_command(t_token **cur)
{
	if (*cur && is_lparen(*cur))
		return (handle_subshell_branch(cur));
	return (handle_plain_command(cur));
}
