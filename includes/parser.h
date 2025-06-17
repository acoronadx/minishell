/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:07:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:36:18 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "lexer.h"
# include "ast.h"
# include "env.h"
# include "exec.h"

typedef struct s_ast t_ast;
typedef struct s_shell t_shell;

// Avanza el puntero de token al siguiente
void	next_token(t_token **cur);

// Funciones de parsing principales
t_ast	*parser_line(t_shell *shell);

// Parsing de la línea en árboles de sintaxis
t_ast	*parse_and_or(t_token **cur);
t_ast	*parse_background(t_token **cur);
t_ast	*parse_sequence(t_token **cur);
t_ast	*parse_pipeline(t_token **cur);
t_ast	*parse_simple_command(t_token **cur);
t_ast	*parse_group(t_token **cur);

// Utilidades para construir nodos AST de comandos/redirecciones
t_ast	*create_command_node(char **argv, t_ast *redir_head);
t_ast	*make_redir_node(t_token *redir, t_token *file);

// Parsing de argumentos y redirecciones
char	**add_argument(char **argv, int *argc, int *size, char *value);
int		parse_redirections(t_token **cur, t_ast **redir_head, t_ast **redir_tail);
char	**parse_arguments(t_token **cur);
int		is_redirection(t_token *tok);

#endif
