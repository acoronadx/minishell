/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:36:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/31 22:12:03 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

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

void	free_token_list(t_token *tok)
{
	t_token	*next;

	while (tok)
	{
		next = tok->next;
		free(tok->value);
		free(tok);
		tok = next;
	}
}

void	next_token(t_token **cur)
{
	if (cur && *cur)
		*cur = (*cur)->next;
}

int	is_operator(const char *str, t_token_type *type, int *len)
{
	static const char		*ops[] = {
		"<<<", ">>", "<<", "||", "&&", "&>>", "&>", ">|", "<&", ">&",
		"2>>", "2>", "|", "&", ";", "<", ">", "(", ")", "{", "}", "="
	};
	static const int		lens[] = {
		3, 2, 2, 2, 2, 3, 2, 2, 2, 2,
		3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	static const t_token_type types[] = {
		T_HEREDOC_STR, T_APPEND, T_HEREDOC, T_OR, T_AND, T_APPEND_ALL,
		T_REDIR_ALL, T_FORCE_OUT, T_DUP_IN, T_DUP_OUT, T_APPEND_ERR,
		T_REDIR_ERR, T_PIPE, T_BG, T_SEMI, T_REDIR_IN, T_REDIR_OUT,
		T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_EQUAL
	};
	int						i;

	i = 0;
	while (i < (int)(sizeof(lens) / sizeof(lens[0]))
		&& i < (int)(sizeof(types) / sizeof(types[0])))
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

void	free_lexer_list_on_error(t_token **lst)
{
	if (lst && *lst)
	{
		free_token_list(*lst);
		*lst = NULL;
	}
}

int	try_add_token(t_token **lst, char *str, t_token_type type, t_quote quote)
{
	t_token	*tok;

	tok = token_new(str, type, quote);
	if (!tok)
	{
		ft_strdel(&str);
		return (0);
	}
	token_addback(lst, tok);
	return (1);
}

static void	copy_escaped_pair(const char *src, size_t *i, char *out, size_t *j)
{
	(*i)++;
	out[*j] = src[*i];
	(*j)++;
	(*i)++;
}

char	*remove_backslashes_for_token(const char *src, t_quote quote)
{
	char	*out;
	size_t	i;
	size_t	j;
	size_t	n;

	if (!src)
		return (NULL);
	n = ft_strlen(src);
	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	if (quote == SINGLE_QUOTE)
	{
		while (i < n)
		{
			out[j] = src[i];
			j++;
			i++;
		}
		out[j] = '\0';
		return (out);
	}
	if (quote == DOUBLE_QUOTE)
	{
		while (i < n)
		{
			if (src[i] == '\\' && (i + 1) < n && (src[i + 1] == '$'
					|| src[i + 1] == '`' || src[i + 1] == '"'
					|| src[i + 1] == '\\'))
				copy_escaped_pair(src, &i, out, &j);
			else if (src[i] == '\\' && (i + 1) < n && src[i + 1] == '\n')
				i += 2;
			else
			{
				out[j] = src[i];
				j++;
				i++;
			}
		}
		out[j] = '\0';
		return (out);
	}
	while (i < n)
	{
		if (src[i] == '\\' && (i + 1) < n && src[i + 1] == '\n')
			i += 2;
		else if (src[i] == '\\' && (i + 1) < n)
			copy_escaped_pair(src, &i, out, &j);
		else
		{
			out[j] = src[i];
			j++;
			i++;
		}
	}
	out[j] = '\0';
	return (out);
}
