/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 06:55:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:35:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "minishell.h"
#include "ast.h"
#include "env.h"
#include "parser.h"

typedef struct s_ast t_ast;
typedef struct s_shell t_shell;

// AND / OR
int		execute_and(t_ast *node, t_shell *shell);
int		execute_or(t_ast *node, t_shell *shell);

// ÁRBOL DE EJECUCIÓN PRINCIPAL
int		execute_ast(t_ast *node, t_shell *shell);

// BACKGROUND & SUBSHELL
int		execute_background(t_ast *node, t_shell *shell);
int		execute_subshell(t_ast *node, t_shell *shell);

// PIPE, SEQUENCE
int		execute_pipe(t_ast *node, t_shell *shell);
int		execute_sequence(t_ast *node, t_shell *shell);

// BUILTINS Y COMANDOS
int		is_builtin(char *cmd);
int		execute_builtin(char **argv, t_shell *shell);

int		execute_command(t_ast *node, t_shell *shell);
int		fork_execve(char **argv, t_shell *shell);

// REDIRECCIONES
int		restore_and_return(int stdin_copy, int stdout_copy, int ret);
int		apply_all_redirections(t_ast *redir, int in, int out);
int		apply_redirection(t_ast *redir);
int		handle_heredoc(char *filename);

// ONE COMMAND, SCRIPT
void	execute_one_command(char *command_str, t_shell *shell);
void	execute_script(const char *filename, t_shell *shell);

// UTILIDADES
void	clean_line(char **line);
void	parse_and_execute(t_shell *shell);
void	cleanup_loop(t_shell *shell);

// EXECUTE COMMAND:
int	apply_redirection(t_ast *redir);
int	is_builtin(char *cmd);
int	execute_builtin(char **argv, t_shell *shell);
int	fork_execve(char **argv, t_shell *shell);

#endif
