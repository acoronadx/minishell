/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_abort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 23:10:00 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 23:29:18 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_hd_list(t_ast *lst)
{
	t_ast	*tmp;

	tmp = lst;
	while (tmp)
	{
		if (tmp->type == N_REDIR
			&& tmp->redir.redir_type == REDIR_HEREDOC
			&& tmp->redir.heredoc_fd >= 0)
		{
			close(tmp->redir.heredoc_fd);
			tmp->redir.heredoc_fd = -1;
		}
		tmp = tmp->next;
	}
}

void	abort_heredocs_ast(t_ast *n)
{
	if (!n)
		return ;
	if (n->type == N_COMMAND)
	{
		close_hd_list(n->cmd.redirections);
		return ;
	}
	if (n->type == N_SUBSHELL)
	{
		abort_heredocs_ast(n->subshell.child);
		close_hd_list(n->subshell.redirections);
		return ;
	}
	if (n->type == N_PIPE || n->type == N_AND || n->type == N_OR
		|| n->type == N_SEQUENCE || n->type == N_BACKGROUND)
	{
		abort_heredocs_ast(n->bin.left);
		abort_heredocs_ast(n->bin.right);
	}
}
