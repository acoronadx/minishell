/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_opertator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 19:03:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_operator(const char *line, int i, t_token **lst)
{
	t_token_type	type;
	int				len;
	char			*op;

	if (!is_operator(line + i, &type, &len))
		return (i);
	op = ft_substr(line, i, len);
	if (!op)
	{
		free_lexer_list_on_error(lst);
		return (-1);
	}
	if (!try_add_token(lst, op, type, NO_QUOTE))
		return (-1);
	return (i + len);
}

void mark_heredoc_delims(t_token *head)
{
    t_token *t = head;

    while (t)
    {
        if (t->type == T_HEREDOC && t->next && t->next->type == T_WORD)
        {
            t->next->is_hdoc_delim   = 1;
            t->next->hdoc_was_quoted = (t->next->quoted != NO_QUOTE);
        }
        t = t->next;
    }
}
