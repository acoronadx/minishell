/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:41 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 16:52:01 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/* Forward declarations para evitar ciclos */
typedef struct s_token t_token;
typedef struct s_ast   t_ast;
typedef struct s_shell t_shell;

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
    REDIR_ERR,
    REDIR_APPEND_ERR,
    REDIR_ALL,
    REDIR_APPEND_ALL,
    REDIR_FORCE,
    REDIR_DUP_IN,
    REDIR_DUP_OUT,
    REDIR_INVALID
}   t_redir_type;

struct s_ast
{
    t_node_type type;
    union
    {
        struct {
            char        **argv;
            struct s_ast *redirections; /* lista ligada de N_REDIR */
        } cmd;
        struct {
            char        *filename;
            char        *delimiter;
            t_redir_type redir_type;
            int         redir_fd;
			int         heredoc_fd;     /* = -1 normal, >=0 si heredoc via pipe */
			int         heredoc_quoted;
        } redir;
        struct {
            struct s_ast *left;
            struct s_ast *right;
        } bin;
        struct {
            struct s_ast *child;
            struct s_ast *redirections;
        } subshell;
    };
};

/* Constructores */
t_ast   *ast_new_command(char **argv, t_ast *redirections);
t_ast   *ast_new_redir(char *filename, char *delimiter,
                       t_redir_type redir_type, int redir_fd);
t_ast   *ast_new_binary(t_node_type type, t_ast *left, t_ast *right);
t_ast   *ast_new_subshell(t_ast *child, t_ast *redirections);
t_ast   *create_command_node(char **argv, t_ast *redir_list_head);
t_ast *ast_new_redir(char *filename, char *delimiter, t_redir_type type,
		int redir_fd);

/* Parser entrypoint (implementado en tu módulo parser) */
t_ast   *build_ast(t_token *tokens);

/* Free ast helpers */
void    free_ast(t_ast *node);
void    free_ast_recursive(t_ast *node);

/* Free ast utils */
void    free_ast_cmd(t_ast *node);
void    free_ast_redir(t_ast *node);
void    free_ast_sub_or_bin(t_ast *node);


#endif
