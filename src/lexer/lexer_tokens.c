/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:36:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 15:44:51 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	next_token(t_token **cur)
{
	if (cur && *cur)
		*cur = (*cur)->next;
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
