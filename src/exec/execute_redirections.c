/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:12:04 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 16:11:40 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	redirect_to(int fd, int std_fd)
{
	int	ret = 0;
	if (dup2(fd, std_fd) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		ret = 1;
	}
	close(fd);
	return (ret);
}

static int	handle_heredoc(t_ast *redir)
{
	int	fd = heredoc_prepare(redir->redir.delimiter);
	if (fd < 0)
		return (1);
	return (redirect_to(fd, STDIN_FILENO));
}

int	open_redirection(t_ast *redir)
{
	int	fd = -1;

	if (!redir || redir->type != N_REDIR) return (-1);
	if (redir->redir.redir_type == REDIR_IN)
		fd = open(redir->redir.filename, O_RDONLY);
	else if (redir->redir.redir_type == REDIR_OUT
		|| redir->redir.redir_type == REDIR_FORCE)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redir.redir_type == REDIR_APPEND)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->redir.redir_type == REDIR_ERR)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redir.redir_type == REDIR_APPEND_ERR)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->redir.redir_type == REDIR_ALL)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redir.redir_type == REDIR_APPEND_ALL)
		fd = open(redir->redir.filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

static int	handle_dup_redir(t_ast *redir)
{
	int	target = redir->redir.redir_fd;
	int	std_fd = (redir->redir.redir_type == REDIR_DUP_OUT) ? STDOUT_FILENO : STDIN_FILENO;

	if (target == std_fd) return (0);
	if (dup2(target, std_fd) < 0)
	{
		perror("minishell: dup2");
		return (1);
	}
	return (0);
}

int	handle_simple_redir(t_ast *redir)
{
	int	fd = open_redirection(redir);
	int	ret = 0;

	if (fd < 0)
	{
		perror(redir->redir.filename);
		return (1);
	}
	if (redir->redir.redir_type == REDIR_IN)
		ret = redirect_to(fd, STDIN_FILENO);
	else if (redir->redir.redir_type == REDIR_OUT
		|| redir->redir.redir_type == REDIR_FORCE
		|| redir->redir.redir_type == REDIR_APPEND)
		ret = redirect_to(fd, STDOUT_FILENO);
	else if (redir->redir.redir_type == REDIR_ERR
		|| redir->redir.redir_type == REDIR_APPEND_ERR)
		ret = redirect_to(fd, STDERR_FILENO);
	else if (redir->redir.redir_type == REDIR_ALL
		|| redir->redir.redir_type == REDIR_APPEND_ALL)
	{
		if (dup2(fd, STDOUT_FILENO) < 0) ret = 1;
		else if (dup2(fd, STDERR_FILENO) < 0) ret = 1;
		close(fd);
	}
	return (ret);
}

int	apply_redirections(t_ast *redir_list)
{
	t_ast	*curr = redir_list;
	int		fail = 0;

	while (curr)
	{
		if (curr->type != N_REDIR)
			return (1);

		if (curr->redir.redir_type == REDIR_HEREDOC)
		{
			if (handle_heredoc(curr))
				fail = 1;
		}
		else if (curr->redir.redir_type == REDIR_DUP_IN
			|| curr->redir.redir_type == REDIR_DUP_OUT)
		{
			if (handle_dup_redir(curr))
				fail = 1;
		}
		else
		{
			if (handle_simple_redir(curr))
				fail = 1;
		}
		curr = curr->bin.right;
	}
	return (fail ? 1 : 0);
}
