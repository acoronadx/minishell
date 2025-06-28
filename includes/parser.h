/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:07:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 03:59:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h" // Asume que incluye t_token, t_ast, etc.
# include "ast.h"        // Necesario para t_ast, ast_new, etc.
# include "lexer.h"      // Necesario para t_token, t_token_type, etc.
# include "env.h"        // Si necesitas acceder a variables de entorno en el parser


// Prototipos de funciones relacionadas con redirecciones (si las tienes)
int     is_redirection(t_token *tok);
// int  is_redirection_type(t_token_type type); // Podría ser estática en el .c o aquí si la necesitas en más sitios
int     parse_redirections(t_token **cur, t_ast **head, t_ast **tail);
t_ast   *make_redir_node(t_token *redir_token, t_token *filename_token); // Si esta es una función externa
t_ast   *parse_sequence(t_token **cur);
t_ast   *parse_and_or(t_token **cur);
t_ast   *parser_line(t_shell *shell);
t_ast   *parse_pipeline(t_token **cur);
t_ast   *parse_background(t_token **cur);
t_ast   *parse_command_and_redirections(t_token **cur);
t_ast   *parse_group(t_token **cur);
char    **parse_arguments(t_token **cur);
char    **add_argument(char **argv, int *argc, int *size, const char *arg);

// Para manejar comandos simples, subshells y argumentos
t_ast   *parse_simple_command(t_token **cur);
t_ast   *parse_group(t_token **cur);
char    **parse_arguments(t_token **cur);

// Para crear el nodo final del comando
t_ast   *create_command_node(char **argv, t_ast *redirections_list_head);

#endif