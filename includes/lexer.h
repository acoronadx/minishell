/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 13:06:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

/* ------------------------- Tipos de token ------------------------- */
typedef enum e_token_type
{
        T_WORD,
        T_PIPE,         // |
        T_OR,           // ||
        T_AND,          // &&
        T_BG,           // &
        T_SEMI,         // ;
        T_REDIR_IN,     // <
        T_REDIR_OUT,    // >
        T_APPEND,       // >>
        T_HEREDOC,      // <<
        T_REDIR_ALL,    // &>
        T_APPEND_ALL,   // &>>
        T_FORCE_OUT,    // >|
        T_HEREDOC_STR,  // <<< (Bash-specific)
        T_DUP_IN,       // <&
        T_DUP_OUT,      // >&
        T_LPAREN,       // (
        T_RPAREN,       // )
        T_UNKNOWN
}       t_token_type;


typedef enum e_quote
{
        NO_QUOTE,
        SINGLE_QUOTE,
        DOUBLE_QUOTE
}       t_quote;

typedef struct s_token
{
        char            *value;     /* texto literal (incluye comillas si las hay) */
        t_token_type    type;
        t_quote         quoted;     /* comillado delimitador del token (si aplica) */
        struct s_token  *next;
}       t_token;

/* ------------------------- API del lexer ------------------------- */
t_token         *lexer(const char *line);

/* Construcción/gestión de lista */
t_token         *token_new(char *value, t_token_type type, t_quote quote);
void            token_addback(t_token **lst, t_token *new);
void            free_token_list(t_token *tok);
void            next_token(t_token **cur);

/* Funciones de extracción */
int             get_operator(const char *line, int i, t_token **lst);
int             get_word(const char *line, int i, t_token **lst);

/* Detección y utilidades */
int             is_operator(const char *str, t_token_type *type, int *len);
t_token_type    get_token_type(const char *str, int len);
t_token_type    get_pipe_and_or(const char *str, int len);
t_token_type    get_semi_redir_left(const char *str, int len);
t_token_type    get_redir_right(const char *str, int len);
t_token_type    get_redir_special(const char *str, int len);

void            free_lexer_list_on_error(t_token **lst);
void			free_token_list(t_token *tok);
t_token			*free_null_token_list(t_token **lst);
int             try_add_token(t_token **lst, char *str, t_token_type type, t_quote quote);

/* Utilidades específicas de bash */
void            strip_comment_if_applicable(char *line);
char            *remove_backslashes_for_token(const char *src, t_quote quote);


/* Utilidades para comentarios */
int		is_comment(char *line, char c, int aws, int i);
int		enter_quote_out(char c, t_quote *q, int *i, int *aws);
void	shift_left_two(char *s, int pos);
int		handle_bs_out(char *line, int *i, int *aws);


#endif
