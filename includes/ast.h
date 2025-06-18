/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:41 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 17:14:50 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "lexer.h"
# include "minishell.h"
# include "env.h"
# include "parser.h"
# include "exec.h"

typedef struct s_ast t_ast;
typedef struct s_shell t_shell;

typedef enum e_node_type
{
	N_COMMAND,   // Nodo comando simple (con argv)
	N_PIPE,      // Nodo pipe (|), left y right apuntan a los comandos a conectar
	N_REDIR,     // Nodo redirección (> < >> <<), filename y redir_type usados
	N_SUBSHELL,   // Nodo agrupación/subshell: ( ... ), left apunta al subárbol
	N_SEQUENCE,  // secuencia: cmd1 ; cmd2
	N_AND,       // and:       cmd1 && cmd2
	N_OR,        // or:        cmd1 || cmd2
	N_BACKGROUND // background: cmd1 & cmd2
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;        // Solo para N_COMMAND
	char			*filename;     // Solo para N_REDIR
	int				redir_type;    // Tipo de redirección
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

// Declaraciones de AST utils
void	ast_free(t_ast *node);
t_ast	*ast_copy(t_ast *node);
t_ast	*ast_new(t_node_type type, char **argv, char *filename, int redir_type,
t_ast	*left, t_ast *right);
char	**ft_strdup_array(char **array);
int		ft_str_array_len(char **array);

// Declaraciones de build_ast
t_ast	*build_ast(t_token *tokens);

int	check_syntax(t_ast *node);


#endif
