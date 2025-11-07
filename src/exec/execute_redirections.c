/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:36:49 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 08:37:51 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Helpers genéricos                                                          */
/* -------------------------------------------------------------------------- */

/*
static int	redirect_to(int fd, int std_fd)
{
	int	ret;

	ret = 0;
	if (dup2(fd, std_fd) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		ret = 1;
	}
	close(fd);
	return (ret);
}
*/

/* Heurística: nuestros tmp de heredoc empiezan así */
static int	is_hd_tmp(const char *p)
{
	if (!p)
		return (0);
	return (ft_strncmp(p, "/tmp/minish_hd_", 15) == 0);
}

static int	open_rdonly(const char *p, int *fd)
{
	*fd = open(p, O_RDONLY);
	if (*fd < 0)
	{
		perror("minishell: open");
		return (1);
	}
	return (0);
}

static int	open_w_trunc(const char *p, int *fd)
{
	*fd = open(p, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
	{
		perror("minishell: open");
		return (1);
	}
	return (0);
}

static int	open_w_append(const char *p, int *fd)
{
	*fd = open(p, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd < 0)
	{
		perror("minishell: open");
		return (1);
	}
	return (0);
}

static int	dup_into(int fd, int target)
{
	if (dup2(fd, target) < 0)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Aplicadores por tipo                                                       */
/* -------------------------------------------------------------------------- */

/* IN: soporta unlink temprano si viene de heredoc */
static int	apply_in(const char *path)
{
	int	fd;
	int	err;

	if (!path || *path == '\0')
	{
		ft_dprintf(2, "minishell: ambiguous redirect\n");
		return (1);
	}
	if (open_rdonly(path, &fd))
		return (1);
	if (is_hd_tmp(path))
		unlink(path);
	err = dup_into(fd, STDIN_FILENO);
	return (err);
}

static int	apply_out_common(const char *path, int to_fd, int append)
{
	int	fd;

	if (!path || *path == '\0')
	{
		ft_dprintf(2, "minishell: ambiguous redirect\n");
		return (1);
	}
	if (append)
	{
		if (open_w_append(path, &fd))
			return (1);
	}
	else
	{
		if (open_w_trunc(path, &fd))
			return (1);
	}
	return (dup_into(fd, to_fd));
}

/* Abrimos una vez y duplicamos a 1 y 2 */
static int	apply_all(const char *path, int append)
{
	int	fd;

	if (!path || *path == '\0')
	{
		ft_dprintf(2, "minishell: ambiguous redirect\n");
		return (1);
	}
	if (append)
	{
		if (open_w_append(path, &fd))
			return (1);
	}
	else
	{
		if (open_w_trunc(path, &fd))
			return (1);
	}
	if (dup_into(dup(fd), STDOUT_FILENO))
	{
		close(fd);
		return (1);
	}
	if (dup_into(fd, STDERR_FILENO))
		return (1);
	return (0);
}

/* Duplicaciones <&N o >&N (usa redir_fd) */
static int	validate_fd(int fd)
{
	/* opcional: limita fds negativos o demasiado grandes */
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: bad file descriptor: %d\n", fd);
		return (0);
	}
	return (1);
}

static int	apply_dup(const t_ast *r)
{
	int	src;

	src = r->redir.redir_fd;
	if (!validate_fd(src))
		return (1);
	if (r->redir.redir_type == REDIR_DUP_IN)
		return (dup_into(dup(src), STDIN_FILENO));
	/* REDIR_DUP_OUT */
	return (dup_into(dup(src), STDOUT_FILENO));
}

/* -------------------------------------------------------------------------- */
/* Nodo individual                                                            */
/* -------------------------------------------------------------------------- */

static int	apply_one_redir(const t_ast *r)
{
	if (r->type != N_REDIR)
		return (0);
	if (r->redir.redir_type == REDIR_HEREDOC)
	{
		/* No debería llegar aquí: prepare_heredocs debió convertirlo a REDIR_IN */
		ft_dprintf(2, "minishell: internal error: heredoc not prepared\n");
		return (1);
	}
	if (r->redir.redir_type == REDIR_IN)
		return (apply_in(r->redir.filename));
	if (r->redir.redir_type == REDIR_OUT)
		return (apply_out_common(r->redir.filename, STDOUT_FILENO, 0));
	if (r->redir.redir_type == REDIR_APPEND)
		return (apply_out_common(r->redir.filename, STDOUT_FILENO, 1));
	if (r->redir.redir_type == REDIR_ERR)
		return (apply_out_common(r->redir.filename, STDERR_FILENO, 0));
	if (r->redir.redir_type == REDIR_APPEND_ERR)
		return (apply_out_common(r->redir.filename, STDERR_FILENO, 1));
	if (r->redir.redir_type == REDIR_ALL)
		return (apply_all(r->redir.filename, 0));
	if (r->redir.redir_type == REDIR_APPEND_ALL)
		return (apply_all(r->redir.filename, 1));
	if (r->redir.redir_type == REDIR_DUP_IN
		|| r->redir.redir_type == REDIR_DUP_OUT)
		return (apply_dup(r));
	/* REDIR_FORCE u otros -> aquí lo tratamos como OUT con flags extra ya en open_w_trunc */
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Lista de redirecciones                                                     */
/* -------------------------------------------------------------------------- */

int	apply_redirections(t_ast *redir_list)
{
	t_ast	*curr;

	curr = redir_list;
	while (curr)
	{
		if (apply_one_redir(curr))
			return (1);
		curr = curr->bin.right;
	}
	return (0);
}
