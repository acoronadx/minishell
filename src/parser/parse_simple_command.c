/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:39:45 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 17:33:01 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_simple_command(t_token **cur)
{
	t_ast	*redir_head = NULL;
	t_ast	*redir_tail = NULL;
	t_ast	*cmd_node;
	char	**argv;

	/* caso subshell: ( ... ) [redirs] */
	if (*cur && (*cur)->type == T_LPAREN)
	{
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

	/* redirecciones iniciales */
	if (!parse_redirections(cur, &redir_head, &redir_tail))
		return (NULL);

/* parse_simple_command.c */

    argv = parse_arguments(cur);
    if (!argv)
    {
        /* === ANTES: error si había redirecciones sin argv ===
        if (redir_head) {
            free_ast(redir_head);
            ft_dprintf(2,"minishell: syntax error: empty command before redirection\n");
            return (NULL);
        }
        ft_dprintf(2,"minishell: syntax error: empty command or unexpected token\n");
        return (NULL);
        */

        /* === AHORA: comportamiento bash — permitir solo redirecciones === */
        if (redir_head)
            return ast_new_command(NULL, redir_head);  /* argv == NULL */
        ft_dprintf(2, "minishell: syntax error: empty command\n");
        return (NULL);
    }

	/* redirecciones finales */
	if (!parse_redirections(cur, &redir_head, &redir_tail))
	{
		ft_free_strtab(argv);
		free_ast(redir_head);
		return (NULL);
	}

	cmd_node = ast_new_command(argv, redir_head);
	if (!cmd_node)
	{
		ft_free_strtab(argv);
		free_ast(redir_head);
		return (NULL);
	}
	return (cmd_node);
}
