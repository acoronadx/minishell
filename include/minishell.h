/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 19:44:12 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 01:17:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

// ------------------------ //
// ESTRUCTURA DE VARIABLES //
// ------------------------ //

typedef struct s_env
{
	char			*key;        // NOMBRE de la variable (ej: "PATH")
	char			*value;      // VALOR de la variable (ej: "/usr/bin")
	int				exported;    // 1 si se ha hecho export, 0 si no
	struct s_env	*next;
}	t_env;

// ------------------------ //
// TIPOS DE TOKENS         //
// ------------------------ //

typedef enum e_token_type
{
	T_WORD,         // palabra normal
	T_PIPE,         // |
	T_REDIR_IN,     // <
	T_REDIR_OUT,    // >
	T_REDIR_APPEND, // >>
	T_HEREDOC,       // <<
    T_SEMICOLON,
    T_AND,
    T_OR,
    T_BACKGROUND,
    T_OPEN_PAREN,
    T_CLOSE_PAREN,
    T_EOF
}	t_token_type;


typedef struct s_token
{
	char			*value;      // contenido del token
	t_token_type	type;        // tipo del token
	int				in_quotes;  // 1 si estaba entre comillas
	int				expand;     // 1 si se debe expandir $VAR
	struct s_token	*next;
}	t_token;


// ------------------------ //
// ESTRUCTURA DE COMANDO    //
// ------------------------ //

typedef struct s_cmd
{
    char        **argv;          // argumentos: argv[0] es el comando
    char        *redir_in;       // fichero para redirección de entrada (< o <<)
    char        *redir_out;      // fichero para redirección de salida (> o >>)
    int         append_out;      // 1 si es >>, 0 si es >
    int         heredoc;         // 1 si redir_in es heredoc (<<)
    struct s_cmd *next;          // siguiente comando en el pipeline
}   t_cmd;


// ------------------------ //
// ESTRUCTURA PRINCIPAL     //
// ------------------------ //

typedef struct s_shell
{
	t_env		*env_list;     // ← NUEVO: lista de entorno como Bash
	char		*line;         // línea actual leída
	t_token		*tokens;       // lista enlazada de tokens
    int			exit_status;  // Último código de salida
	int			stdin_backup;  // copia de stdin
	int			stdout_backup; // copia de stdout
}	t_shell;


// ------------------------ //
// FUNCIONES DEL SHELL      //
// ------------------------ //

// Inicialización
void		init_shell(t_shell *shell, char **envp);
t_env		*init_env_list(char **envp);
t_env		*env_create(char *key, char *value, int exported);
void		env_add_back(t_env **lst, t_env *new);
void		cleanup_shell(t_shell *shell);
void		cleanup_env_list(t_env *env);

// Parsing y ejecución
void		parse_and_execute(t_shell *shell, char *line);
void		parse_line(t_shell *shell, char *line);
char		*ft_strinquotes(char *str, int *expand);
void		ft_add_token(t_shell *shell, char *value, t_token_type type, int in_quotes, int expand);
int			ft_add_redirection(t_shell *shell, char *line);
int			ft_add_word(t_shell *shell, char *line);
t_cmd		*cmd_new(void);
t_cmd		*parse_commands(t_token *tokens);
int			parse_redirections(t_cmd *cmd, t_token **tokens);
t_cmd		*free_cmds(t_cmd *cmd);
t_cmd		*check_pipe_syntax(t_token *tokens, t_cmd *head);

// Tokens
void		free_tokens(char **tokens);
int			ft_ispipe(int c);
int			ft_isenvvar(int c);
int			ft_isredirection(int c);
int			ft_isspecial_in_double_quotes(char c);

// Expansión
char	*expand_value(t_shell *shell, char *str);
void	expand_tokens(t_shell *shell);

// Shell exec - env_utils.c - utils
char		*get_env_path(t_env *env_list, const char *cmd);
char		*get_env_value(t_env *env, const char *key);
char		*get_env_value_any(t_env *env, const char *key);
t_env		*get_env_node(t_env *env_list, const char *key);
void		env_remove_key(t_env **env, const char *key);
int			is_valid_identifier(const char *str);

// Print errors
void		ft_print_exit_init_error(char *msg, t_shell *shell);
void		ft_print_exit_user_error(t_shell *shell, const char *msg, int code);
void		ft_print_exit_sys_error(t_shell *shell, const char *msg, int code);

// Builtins - builtins_simple.c
int			builtin_echo(char **args);
int			builtin_pwd(char **args);
int			builtin_env(char **envp, char **args);
int			builtin_exit(char **args, t_shell *shell);

// Builtins -  builtins_env.c
int			builtin_cd(char **args, t_shell *shell);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);

// dispatcher
int			exec_builtin(char **args, t_shell *shell);

char		**ft_split_custom(const char *s, char c);
void		print_prompt(void);
char		*expand_dollar_question(const char *line, int last_status);

#endif
