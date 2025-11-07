/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 23:10:00 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 23:28:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_one_hd(t_ast *r, t_shell *sh)
{
	int	p[2];
	int	rc;

	rc = pipe(p);
	if (rc < 0)
		return (-1);
	rc = hd_loop(sh, p[1], r->redir.delimiter, r->redir.heredoc_quoted);
	if (rc != 0)
	{
		close(p[0]);
		close(p[1]);
		return (-1);
	}
	close(p[1]);
	r->redir.heredoc_fd = p[0];
	return (0);
}

static int	prepare_redir_list(t_ast *list, t_shell *sh)
{
	t_ast	*cur;
	int		rc;

	cur = list;
	while (cur)
	{
		if (cur->type == N_REDIR
			&& cur->redir.redir_type == REDIR_HEREDOC)
		{
			rc = setup_one_hd(cur, sh);
			if (rc != 0)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	prepare_node(t_ast *n, t_shell *sh)
{
	int	rc;

	if (!n)
		return (0);
	if (n->type == N_COMMAND)
		return (prepare_redir_list(n->cmd.redirections, sh));
	if (n->type == N_SUBSHELL)
	{
		rc = prepare_redir_list(n->subshell.redirections, sh);
		if (rc != 0)
			return (-1);
		return (prepare_node(n->subshell.child, sh));
	}
	if (n->type == N_PIPE || n->type == N_AND || n->type == N_OR
		|| n->type == N_SEQUENCE || n->type == N_BACKGROUND)
	{
		rc = prepare_node(n->bin.left, sh);
		if (rc != 0)
			return (-1);
		return (prepare_node(n->bin.right, sh));
	}
	return (0);
}

int	prepare_heredocs(t_ast *root, t_shell *sh)
{
	int	rc;

	rc = prepare_node(root, sh);
	if (rc != 0 && sh->exit_status == 0)
		sh->exit_status = 130;
	return (rc);
}
