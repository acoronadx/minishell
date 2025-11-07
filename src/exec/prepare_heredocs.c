/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:41:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 17:12:13 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Señales para modo heredoc                                                  */
/* -------------------------------------------------------------------------- */

static void	hd_sigint(int sig)
{
	(void)sig;
	g_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_done = 1;
}

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

/* -------------------------------------------------------------------------- */
/* Expansión de línea en heredoc: solo $..., si el delimitador NO estaba quote*/
/* -------------------------------------------------------------------------- */

static char	*hd_expand_line_opt(char *line, t_shell *sh, int quoted)
{
	char	*out;

	if (quoted)
		return (line);
	/* Usa tu API de expansión ligera: solo $..., sin split ni glob */
	out = expand_heredoc_line(line, sh);
	if (!out)
		return (NULL);
	free(line);
	return (out);
}

/* -------------------------------------------------------------------------- */
/* Entrada de texto del heredoc -> escribe en el extremo de escritura del pipe*/
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/* HEREDOC -> PIPE: crea pipe, escribe y convierte el nodo a REDIR_IN (FD)    */
/* -------------------------------------------------------------------------- */

static int	prepare_one_heredoc_node(t_ast *redir, t_shell *sh)
{
	int	p[2];
	int	ok;

	if (pipe(p) < 0)
		return (0);
	/* p[0] lectura -> se quedará en el AST; p[1] escritura -> se usa aquí */
	ok = hd_write_to_fd(p[1], redir->redir.delimiter,
			redir->redir.heredoc_quoted, sh);
	close(p[1]);
	if (!ok)
	{
		/* Ctrl-C o error: cerramos lectura y fijamos status 130 */
		close(p[0]);
		sh->exit_status = 130;
		return (0);
	}
	/* Convertir el nodo: HEREDOC -> IN con FD interno (sin filename) */
	redir->redir.redir_type = REDIR_IN;
	if (redir->redir.delimiter)
	{
		free(redir->redir.delimiter);
		redir->redir.delimiter = NULL;
	}
	if (redir->redir.filename)
	{
		free(redir->redir.filename);
		redir->redir.filename = NULL;
	}
	redir->redir.heredoc_fd = p[0]; /* NUEVO: lectura del pipe para stdin */
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Recorre la lista de redirecciones (bin.right) y prepara heredocs           */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/* Recorrido del AST izquierda->derecha                                       */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/* API pública                                                                */
/* -------------------------------------------------------------------------- */

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
