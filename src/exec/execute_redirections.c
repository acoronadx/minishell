/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:36:49 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 17:45:50 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */
/* Normaliza filename de redirección:
   - Split con comillas respetadas (no glob)
   - 0 campos  -> *out=NULL, return 0  (dejar que open falle en el caller o tratarlo como ambiguo)
   - >1 campos -> ambiguous (return 2)
   - 1 campo   -> quote-removal y devuelve copia en *out (return 1)
*/
static int normalize_redir_filename(const char *raw, char **out_clean)
{
    t_token *lst;
    t_token *nxt;
    int      count;
    char    *clean;

    *out_clean = NULL;
    if (!raw || *raw == '\0')
        return 0; /* vacío */

    lst = split_word_respecting_quotes(raw);
    if (!lst)
        return 0; /* 0 campos tras split */

    count = 0;
    nxt = lst;
    while (nxt)
    {
        count++;
        nxt = nxt->next;
        if (count > 1)
        {
            free_token_list(lst);
            return 2; /* ambiguous */
        }
    }
    /* 1 campo: quote-removal */
    clean = quote_remove_like_bash(lst->value);
    free_token_list(lst);
    if (!clean)
        return 0;
    *out_clean = clean;
    return 1;
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

static int	open_w_trunc(const char *p, int *fd, int force)
{
	int	flags;

	(void)force; /* si implementas 'noclobber', úsalo aquí */
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	*fd = open(p, flags, 0644);
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

static int	validate_fd(int fd)
{
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: bad file descriptor: %d\n", fd);
		return (0);
	}
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Aplicadores                                                                */
/* -------------------------------------------------------------------------- */

static int apply_in(const char *path_raw)
{
    int   fd;
    char *path;

    switch (normalize_redir_filename(path_raw, &path))
    {
        case 0: /* vacío */
            ft_dprintf(2, "minishell: : No such file or directory\n");
            return 1;
        case 2: /* ambiguous */
            ft_dprintf(2, "minishell: ambiguous redirect\n");
            return 1;
    }
    if (open_rdonly(path, &fd))
    {
        free(path);
        return 1;
    }
    free(path);
    return (dup_into(fd, STDIN_FILENO));
}

static int	apply_in_fd(int fd)
{
	if (fd < 0)
		return (1);
	return (dup_into(fd, STDIN_FILENO));
}

static int apply_out_fd(const char *path_raw, int to_fd, int append, int force)
{
    int   fd;
    char *path;

    switch (normalize_redir_filename(path_raw, &path))
    {
        case 0:
            ft_dprintf(2, "minishell: : No such file or directory\n");
            return 1;
        case 2:
            ft_dprintf(2, "minishell: ambiguous redirect\n");
            return 1;
    }
    if (append)
    {
        if (open_w_append(path, &fd)) { free(path); return 1; }
    }
    else
    {
        if (open_w_trunc(path, &fd, force)) { free(path); return 1; }
    }
    free(path);
    return (dup_into(fd, to_fd));
}


static int apply_all(const char *path_raw, int append, int force)
{
    int   fd;
    char *path;

    switch (normalize_redir_filename(path_raw, &path))
    {
        case 0:
            ft_dprintf(2, "minishell: : No such file or directory\n");
            return 1;
        case 2:
            ft_dprintf(2, "minishell: ambiguous redirect\n");
            return 1;
    }
    if (append)
    {
        if (open_w_append(path, &fd)) { free(path); return 1; }
    }
    else
    {
        if (open_w_trunc(path, &fd, force)) { free(path); return 1; }
    }
    if (dup_into(dup(fd), STDOUT_FILENO)) { close(fd); free(path); return 1; }
    if (dup_into(fd, STDERR_FILENO)) { free(path); return 1; }
    free(path);
    return 0;
}

static int	apply_dup(const t_ast *r)
{
	int	src;

	src = r->redir.redir_fd;
	if (!validate_fd(src))
		return (1);
	if (r->redir.redir_type == REDIR_DUP_IN)
		return (dup_into(dup(src), STDIN_FILENO));
	return (dup_into(dup(src), STDOUT_FILENO));
}

/* -------------------------------------------------------------------------- */
/* Nodo individual                                                            */
/* -------------------------------------------------------------------------- */

static int	apply_one_redir(const t_ast *r)
{
	if (!r || r->type != N_REDIR)
		return (0);

	if (r->redir.redir_type == REDIR_HEREDOC)
	{
		/* prepare_heredocs debió convertirlo a REDIR_IN con heredoc_fd */
		ft_dprintf(2, "minishell: internal error: heredoc not prepared\n");
		return (1);
	}
	if (r->redir.redir_type == REDIR_IN)
	{
		/* Soporte heredoc por pipe: si heredoc_fd >= 0, úsalo */
		if (r->redir.heredoc_fd >= 0)
			return (apply_in_fd(r->redir.heredoc_fd));
		return (apply_in(r->redir.filename));
	}
	if (r->redir.redir_type == REDIR_OUT)
		return (apply_out_fd(r->redir.filename, STDOUT_FILENO, 0, 0));
	if (r->redir.redir_type == REDIR_APPEND)
		return (apply_out_fd(r->redir.filename, STDOUT_FILENO, 1, 0));
	if (r->redir.redir_type == REDIR_ERR)
		return (apply_out_fd(r->redir.filename, STDERR_FILENO, 0, 0));
	if (r->redir.redir_type == REDIR_APPEND_ERR)
		return (apply_out_fd(r->redir.filename, STDERR_FILENO, 1, 0));
	if (r->redir.redir_type == REDIR_ALL)
		return (apply_all(r->redir.filename, 0, 0));
	if (r->redir.redir_type == REDIR_APPEND_ALL)
		return (apply_all(r->redir.filename, 1, 0));
	if (r->redir.redir_type == REDIR_FORCE)
		return (apply_out_fd(r->redir.filename, STDOUT_FILENO, 0, 1));
	if (r->redir.redir_type == REDIR_DUP_IN
		|| r->redir.redir_type == REDIR_DUP_OUT)
		return (apply_dup(r));
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Lista de redirecciones                                                     */
/* -------------------------------------------------------------------------- */

int	apply_redirections(t_ast *redir_list)
{
	t_ast	*cur;

	cur = redir_list;
	while (cur)
	{
		if (apply_one_redir(cur))
			return (1);
		cur = cur->bin.right;
	}
	return (0);
}
