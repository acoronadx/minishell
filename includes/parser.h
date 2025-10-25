/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:07:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 15:27:05 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

/* Evitamos ciclos: no necesitamos lexer.h aquí si solo usamos punteros. */
typedef struct s_token t_token;   /* <- forward declaration */
typedef struct s_ast   t_ast;
typedef struct s_shell t_shell;

# include "minishell.h"
# include "ast.h"
# include "env.h"

/* Prototipos relacionados con redirecciones y parsing */
int     is_redirection(t_token *tok);
int     parse_redirections(t_token **cur, t_ast **head, t_ast **tail);
t_ast   *make_redir_node(t_token *redir_token, t_token *filename_token);
t_ast   *parse_sequence(t_token **cur);
t_ast   *parse_and_or(t_token **cur);
t_ast   *parser_line(t_shell *shell);
t_ast   *parse_pipeline(t_token **cur);
t_ast   *parse_background(t_token **cur);
t_ast   *parse_command_and_redirections(t_token **cur);
t_ast   *parse_subshell(t_token **cur);
char    **parse_arguments(t_token **cur);

/* Comandos simples / subshells */
t_ast   *parse_simple_command(t_token **cur);
t_ast   *parse_subshell(t_token **cur);
char    **parse_arguments(t_token **cur);

#endif
