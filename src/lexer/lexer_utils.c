/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:36:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:57:04 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crea un nuevo token con el valor, tipo y tipo de comillas dados.
** value: cadena dinámica (normalmente salida de ft_substr)
** type: tipo de token, según el enum t_token_type
** quote: tipo de comillas, según el enum t_quote
** Devuelve: puntero al nuevo token, o NULL si falla malloc.
*/
t_token	*token_new(char *value, t_token_type type, t_quote quote)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = value;
	tok->type = type;
	tok->quoted = quote;
	tok->next = NULL;
	return (tok);
}

/*
** Añade un token al final de la lista enlazada.
** Si la lista está vacía, pone el nuevo token como primero.
*/
void	token_addback(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
** Decide el tipo del token dado un string y su longitud.
** Llama a funciones auxiliares para cada familia de operadores.
** Devuelve el tipo específico o T_WORD si no es operador.
*/
t_token_type	get_token_type(const char *str, int len)
{
	t_token_type	type;

	type = get_pipe_and_or(str, len);
	if (type != T_UNKNOWN)
		return (type);
	type = get_semi_redir_left(str, len);
	if (type != T_UNKNOWN)
		return (type);
	type = get_redir_right(str, len);
	if (type != T_UNKNOWN)
		return (type);
	type = get_redir_special(str, len);
	if (type != T_UNKNOWN)
		return (type);
	return (T_WORD);
}

#include "minishell.h"

/*
** Detecta si el string actual comienza un operador bash o símbolo especial.
** Si es así, devuelve 1, pone el tipo en *type y longitud en *len.
** Si no lo es, devuelve 0.
*/
int	is_operator(const char *str, t_token_type *type, int *len)
{
	int i;
	const char *ops[] = {"<<<", ">>", "<<", "||", "&&", "&>>", "&>", ">|", "<&",
		">&", "2>>", "2>", "|", "&", ";", "<", ">", "(", ")", "{", "}", "="};
	const int lens[] = {3,2,2,2,2,3,2,2,2,2,3,2,1,1,1,1,1,1,1,1,1,1};
	const t_token_type types[] = {
		T_HEREDOC_STR, T_APPEND, T_HEREDOC, T_OR, T_AND, T_APPEND_ALL,
		T_REDIR_ALL, T_FORCE_OUT, T_DUP_IN, T_DUP_OUT, T_APPEND_ERR,
		T_REDIR_ERR, T_PIPE, T_BG, T_SEMI, T_REDIR_IN, T_REDIR_OUT,
		T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_EQUAL};

	i = 0;
	while (i < 22)
	{
		if (ft_strncmp(str, ops[i], lens[i]) == 0)
		{
			if (type)
				*type = types[i];
			if (len)
				*len = lens[i];
			return (1);
		}
		i++;
	}
	return (0);
}
