/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:41 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 04:40:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "lexer.h"      // t_token_type si usas tokens
# include "minishell.h"  // t_shell y utilidades
# include "env.h"        // t_env si necesario
# include "parser.h"     // funciones parser

typedef struct s_ast t_ast;
typedef struct s_shell t_shell; // forward declaration

typedef enum e_node_type
{
    N_COMMAND,
    N_PIPE,
    N_REDIR,
    N_SUBSHELL,
    N_SEQUENCE,
    N_AND,
    N_OR,
    N_BACKGROUND
}   t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_ERR,       // nuevo: para 2>
	REDIR_APPEND_ERR,// nuevo: para 2>>
	REDIR_ALL,       // nuevo: para &>
	REDIR_APPEND_ALL,// nuevo: para &>>
	REDIR_FORCE,     // nuevo: para >|
	REDIR_DUP_IN,    // nuevo: para <&
	REDIR_DUP_OUT,   // nuevo: para >&
	REDIR_INVALID
}	t_redir_type;


typedef struct s_ast
{
    t_node_type type;

    union
    {
        struct
        {
            char        **argv;
            struct s_ast *redirections; // lista ligada de N_REDIR
        } cmd;

        struct
        {
            char        *filename;
            char        *delimiter;
            t_redir_type redir_type;
            int         redir_fd;
        } redir;

        struct
        {
            struct s_ast *left;
            struct s_ast *right;
        } bin;

        struct
        {
            struct s_ast *child;
            struct s_ast *redirections;
        } subshell;
    };
}   t_ast;

// Funciones constructoras claras y norm-compliant (máx 4 parámetros)

t_ast   *ast_new_command(char **argv, t_ast *redirections);
t_ast   *ast_new_redir(char *filename, char *delimiter,
                       t_redir_type redir_type, int redir_fd);
t_ast   *ast_new_binary(t_node_type type, t_ast *left, t_ast *right);
t_ast   *ast_new_subshell(t_ast *child, t_ast *redirections);
t_ast	*create_command_node(char **argv, t_ast *redir_list_head);

// Otros prototipos útiles
t_ast   *ast_copy(t_ast *node);
void    free_ast(t_ast *node);
void    free_ast_recursive(t_ast *node);
int     check_syntax(t_ast *node);
int		is_lparen(t_token *tok);

t_ast   *build_ast(t_token *tokens);

#endif
