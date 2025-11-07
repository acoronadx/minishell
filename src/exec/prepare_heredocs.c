/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 08:15:00 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 08:41:49 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* --- Señales específicas del modo heredoc (sin nuevas globals) --- */
static void hd_sigint(int sig)
{
	(void)sig;
	g_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
}

static void hd_push_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	struct sigaction sa;

	g_signal = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = hd_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	signal(SIGQUIT, SIG_IGN);
}

static void hd_pop_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

/* --- Hook de expansión línea a línea: solo si !quoted (opcional) --- */
static char *hd_expand_line_opt(char *line, t_shell *sh, int quoted)
{
	(void)sh;
	if (quoted)
		return (line);
	/* Si ya tienes expansión: char *x = expand_line(line, sh->env); free(line); return x; */
	return (line); /* por ahora, literal */
}

/* --- Escribe el heredoc al fd hasta 'delim' o Ctrl-C --- */
static int hd_write_to_fd(int fdw, const char *delim, int quoted, t_shell *sh)
{
	char *line;

	while (!g_signal)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
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

/* --- Crea tmpfile /tmp/minish_hd_XXXXXX y devuelve fd --- */
static int hd_mktemp(char *path, size_t cap)
{
	int fd;

	if (cap < 32)
		return (-1);
	ft_strlcpy(path, "/tmp/minish_hd_XXXXXX", cap);
	fd = mkstemp(path);
	return (fd);
}

/* --- Convierte HEREDOC -> IN, moviendo campos al nodo --- */
static int ast_redir_convert_to_in(t_ast *r, const char *tmp_path)
{
	char *dup;

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
		free(r->redir.filename);
	r->redir.filename = dup;
	return (1);
}

/* --- Resuelve UN heredoc (crear tmp, leer, convertir nodo) --- */
static int prepare_one_heredoc_node(t_ast *redir, t_shell *sh)
{
	char path[PATH_MAX];
	int  fd;
	int  ok;

	fd = hd_mktemp(path, sizeof(path));
	if (fd < 0)
		return (0);
	ok = hd_write_to_fd(fd, redir->redir.delimiter,
	                    redir->redir.heredoc_quoted, sh);
	close(fd);
	if (!ok)
	{
		unlink(path);
		sh->exit_status = 130; /* Ctrl-C en heredoc */
		return (0);
	}
	if (!ast_redir_convert_to_in(redir, path))
	{
		unlink(path);
		return (0);
	}
	/* Nota: puedes unlinkear tras open() en apply_redirections() para limpieza temprana */
	return (1);
}

/* --- Itera la lista de redirecciones de un comando/subshell --- */
static int prepare_redirs_list(t_ast *head, t_shell *sh)
{
	t_ast *r;

	r = head;
	while (r)
	{
		if (r->type == N_REDIR && r->redir.redir_type == REDIR_HEREDOC)
		{
			if (!prepare_one_heredoc_node(r, sh))
				return (0);
		}
		r = r->bin.right; /* lista enlazada en bin.right */
	}
	return (1);
}

/* --- Recorrido en orden izquierda→derecha por el AST --- */
static int walk_prepare_heredocs(t_ast *n, t_shell *sh)
{
	if (!n)
		return (1);
	if (n->type == N_PIPE || n->type == N_SEQUENCE
	 || n->type == N_AND  || n->type == N_OR
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
	{
		/* Por si llamaras con una lista suelta */
		return (prepare_redirs_list(n, sh));
	}
	return (1);
}

/* --- API pública: prepara TODOS los heredocs del árbol --- */
int prepare_heredocs(t_ast *root, t_shell *sh)
{
	struct sigaction old_int;
	struct sigaction old_quit;
	int ok;

	hd_push_signals(&old_int, &old_quit);
	ok = walk_prepare_heredocs(root, sh);
	hd_pop_signals(&old_int, &old_quit);
	return (ok);
}
