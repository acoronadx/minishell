/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:05:09 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 06:05:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirection(t_ast *redir)
{
	int	fd;

	fd = -1;
	if (redir->redir_type == T_HEREDOC)
		fd = handle_heredoc(redir->filename);
	else if (redir->redir_type == T_REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->redir_type == T_REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redir_type == T_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (1);
	if (redir->redir_type == T_REDIR_IN || redir->redir_type == T_HEREDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	execute_command(t_ast *node, t_shell *shell)
{
	t_ast	*redir;
	int		status;

	if (!node || !node->argv || !node->argv[0])
		return (0);
	redir = node->right;
	while (redir)
	{
		if (apply_redirection(redir) != 0)
			return (1);
		redir = redir->right;
	}
	if (is_builtin(node->argv[0]))
		return (execute_builtin(node->argv, shell));
	status = fork_execve(node->argv, shell);
	return (status);
}
