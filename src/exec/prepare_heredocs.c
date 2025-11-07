/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 15:47:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -- Signal handling (heredoc mode) ---------------------------------------- */

/* Minimal stub: duplicate line without expansion (wire real expansion later). */
static char	*expand_for_heredoc(char *line, t_shell *sh)
{
	(void)sh;
	return (ft_strdup(line));
}


static void	hd_sigint(int sig)
{
	(void)sig;
	g_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
}

/* Install SIGINT custom handler, ignore SIGQUIT. */
static void	hd_push_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	struct sigaction	sa;

	g_signal = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = hd_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_int);

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, old_quit);
}

static void	hd_pop_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

/* -- Optional line expansion: only when not quoted ------------------------- */

static char	*hd_expand_line_opt(char *line, t_shell *sh, int quoted)
{
	char	*out;

	if (quoted)
		return (line);
	out = expand_for_heredoc(line, sh); /* currently duplicates 'line' */
	if (!out)
		return (NULL);
	free(line);
	return (out);
}


/* -- Unique tmp path (no mkstemp): /tmp/minish_hd_<pid>_<ctr> -------------- */

static int	hd_make_tmp(int *out_fd, char **out_path)
{
	static unsigned int	ctr;
	char				*pid_s;
	char				*ctr_s;
	char				*tmp;
	char				*name;
	int					fd;

	pid_s = ft_itoa((int)getpid());
	if (!pid_s)
		return (0);
	ctr_s = ft_itoa((int)ctr++);
	if (!ctr_s)
	{
		free(pid_s);
		return (0);
	}
	/* name = "/tmp/minish_hd_" + pid + "_" */
	tmp = ft_strjoin("/tmp/minish_hd_", pid_s);
	free(pid_s);
	if (!tmp)
	{
		free(ctr_s);
		return (0);
	}
	name = ft_strjoin(tmp, "_");
	free(tmp);
	if (!name)
	{
		free(ctr_s);
		return (0);
	}
	/* *out_path = name + ctr_s */
	*out_path = ft_strjoin(name, ctr_s);
	free(name);
	free(ctr_s);
	if (!*out_path)
		return (0);

	fd = open(*out_path, O_CREAT | O_EXCL | O_WRONLY, 0600);
	if (fd < 0)
	{
		free(*out_path);
		*out_path = NULL;
		return (0);
	}
	*out_fd = fd;
	return (1);
}

/* -- Write heredoc until delimiter or Ctrl-C -------------------------------- */

static int	hd_write_to_fd(int fdw, const char *delim, int quoted, t_shell *sh)
{
	char	*line;

	while (!g_signal)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		line = hd_expand_line_opt(line, sh, quoted);
		if (!line)
			return (0);
		write(fdw, line, ft_strlen(line));
		write(fdw, "\n", 1);
		free(line);
	}
	return (g_signal ? 0 : 1);
}

/* -- Convert HEREDOC redirection node into an input redirection ------------- */

static int	ast_redir_convert_to_in(t_ast *r, const char *tmp_path)
{
	char	*dup;

	dup = ft_strdup(tmp_path);
	if (!dup)
		return (0);
	r->redir.redir_type = REDIR_IN;
	if (r->redir.delimiter)
	{
		free(r->redir.delimiter);
		r->redir.delimiter = NULL;
	}
	if (r->redir.filename)
	{
		free(r->redir.filename);
		r->redir.filename = NULL;
	}
	r->redir.filename = dup;
	return (1);
}

/* -- Handle a single heredoc node ------------------------------------------ */

static int	prepare_one_heredoc_node(t_ast *redir, t_shell *sh)
{
	char	*path;
	int		fd;
	int		ok;

	path = NULL;
	if (!hd_make_tmp(&fd, &path))
		return (0);
	ok = hd_write_to_fd(fd, redir->redir.delimiter,
			redir->redir.heredoc_quoted, sh);
	close(fd);
	if (!ok)
	{
		if (path)
			unlink(path);
		free(path);
		sh->exit_status = 130;
		return (0);
	}
	if (!ast_redir_convert_to_in(redir, path))
	{
		unlink(path);
		free(path);
		return (0);
	}
	/* Keep path for later unlink after open() in apply_redirections(). */
	free(path);
	return (1);
}

/* -- Iterate redirection list (stored in bin.right as a chain) -------------- */

static int	prepare_redirs_list(t_ast *head, t_shell *sh)
{
	t_ast	*r;

	r = head;
	while (r)
	{
		if (r->type == N_REDIR && r->redir.redir_type == REDIR_HEREDOC)
		{
			if (!prepare_one_heredoc_node(r, sh))
				return (0);
		}
		r = r->bin.right;
	}
	return (1);
}

/* -- AST walk left-to-right ------------------------------------------------- */

static int	walk_prepare_heredocs(t_ast *n, t_shell *sh)
{
	if (!n)
		return (1);
	if (n->type == N_PIPE || n->type == N_SEQUENCE
		|| n->type == N_AND || n->type == N_OR
		|| n->type == N_BACKGROUND)
	{
		if (!walk_prepare_heredocs(n->bin.left, sh))
			return (0);
		return (walk_prepare_heredocs(n->bin.right, sh));
	}
	if (n->type == N_SUBSHELL)
	{
		if (!walk_prepare_heredocs(n->subshell.child, sh))
			return (0);
		return (prepare_redirs_list(n->subshell.redirections, sh));
	}
	if (n->type == N_COMMAND)
		return (prepare_redirs_list(n->cmd.redirections, sh));
	if (n->type == N_REDIR)
		return (prepare_redirs_list(n, sh));
	return (1);
}

/* -- Public API ------------------------------------------------------------- */

int	prepare_heredocs(t_ast *root, t_shell *sh)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	int					ok;

	hd_push_signals(&old_int, &old_quit);
	ok = walk_prepare_heredocs(root, sh);
	hd_pop_signals(&old_int, &old_quit);
	return (ok);
}
