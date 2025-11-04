/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:08:04 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:04:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "ast.h"
# include <sys/types.h>

/* Ejecución del AST */
int	execute_ast(t_ast *node, t_shell *shell);
int	execute_command(t_ast *node, t_shell *shell);
int	execute_pipe(t_ast *node, t_shell *shell);
int	execute_and(t_ast *node, t_shell *shell);
int	execute_or(t_ast *node, t_shell *shell);
int	execute_subshell(t_ast *node, t_shell *shell);
int	execute_sequence(t_ast *node, t_shell *shell);
int	execute_background(t_ast *node, t_shell *shell);

/* Redirecciones */
int	open_redirection(t_ast *redir);
int	handle_simple_redir(t_ast *redir);
int	apply_redirections(t_ast *redir_list);

/* Heredoc */
int	heredoc_prepare(const char *delimiter);

/* Auxiliares */
void	restore_std_fds(int original_stdin, int original_stdout, int original_stderr);
char	*join_path(const char *dir, const char *file);
int		is_executable(char *path);
char	*find_executable(char *cmd, t_shell *shell);

/* Errores */
int		print_exec_error(t_shell *shell, const char *cmd, int err_code);

#endif
