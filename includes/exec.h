/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:08:04 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 15:34:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "ast.h" // Para t_ast
# include <sys/types.h> // Por si necesitas pid_t

// Función principal de ejecución del AST
int	execute_ast(t_ast *node, t_shell *shell);

// Funciones internas específicas para nodos del AST
int	execute_ast(t_ast *node, t_shell *shell);
int	execute_command(t_ast *node, t_shell *shell);
int	execute_pipe(t_ast *node, t_shell *shell);
int	execute_and(t_ast *node, t_shell *shell);
int	execute_or(t_ast *node, t_shell *shell);
int	execute_subshell(t_ast *node, t_shell *shell);
int	execute_sequence(t_ast *node, t_shell *shell);
int	execute_background(t_ast *node, t_shell *shell);
int	handle_simple_redir(t_ast *redir);
int	open_redirection(t_ast *redir);
int	apply_redirections(t_ast *redir_list);
int	heredoc_prepare(const char *delimiter);
int	heredoc_read(char *delimiter);

// Funciones auxiliares para la ejecución
void	restore_std_fds(int original_stdin, int original_stdout, int original_stderr);
char	*join_path(const char *dir, const char *file);
int		is_executable(char *path);
char	*find_executable(char *cmd, t_shell *shell);

// Función para imprimir errores de ejecución
int	print_exec_error(t_shell *shell, const char *cmd, int err_code);

#endif
