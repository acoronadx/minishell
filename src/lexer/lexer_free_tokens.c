/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_free_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:43:49 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 15:45:27 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helper: libera lista y devuelve NULL (para early-return) */
t_token	*free_null_token_list(t_token **lst)
{
	free_token_list(*lst);
	*lst = NULL;
	return (NULL);
}

void	free_lexer_list_on_error(t_token **lst)
{
	if (lst && *lst)
	{
		free_token_list(*lst);
		*lst = NULL;
	}
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
