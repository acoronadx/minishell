/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:54:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

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
	T_REDIR_ERR,    // 2>
	T_APPEND_ERR,   // 2>>
	T_REDIR_ALL,    // &>
	T_APPEND_ALL,   // &>>
	T_FORCE_OUT,    // >|
	T_HEREDOC_STR,  // <<<
	T_DUP_IN,       // <&
	T_DUP_OUT,      // >&
	T_LPAREN,       // (
	T_RPAREN,       // )
	T_LBRACE,       // {
	T_RBRACE,       // }
	T_EQUAL,        // =
	T_UNKNOWN
}	t_token_type;

typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote;

typedef struct s_token
{
	char			*value;         // texto del token
	t_token_type	type;           // tipo de token
	t_quote			quoted;         // tipo de comillas
	struct s_token	*next;          // siguiente token
}	t_token;

#endif
