/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 05:05:09 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 07:49:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	restore_and_return(int stdin_copy, int stdout_copy, int ret)
{
	dup2(stdin_copy, 0);
	dup2(stdout_copy, 1);
	close(stdin_copy);
	close(stdout_copy);
	return (ret);
}

int	apply_all_redirections(t_ast *redir, int in, int out)
{
	while (redir)
	{
		if (apply_redirection(redir) != 0)
			return (restore_and_return(in, out, 1));
		redir = redir->right;
	}
	return (0);
}

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
	int		stdin_copy;
	int		stdout_copy;
	int		status;

	if (!node || !node->argv || !node->argv[0])
		return (0);
	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (apply_all_redirections(node->right, stdin_copy, stdout_copy))
		return (1);
	/*if (is_builtin(node->argv[0]))
	{
		status = execute_builtin(node->argv, shell);
		dup2(stdin_copy, 0);
		dup2(stdout_copy, 1);
		close(stdin_copy);
		close(stdout_copy);
		return (status);
	}
		*/
	close(stdin_copy);
	close(stdout_copy);
	status = fork_execve(node->argv, shell);
	return (status);
}
