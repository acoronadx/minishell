/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_free_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:43:49 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 14:32:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lexer_list_on_error(t_token **lst)
{
	if (lst && *lst)
	{
		free_token_list(*lst);
		*lst = NULL;
	}
}

t_token	*free_null_token_list(t_token **lst)
{
	free_lexer_list_on_error(lst);
	return (NULL);
}
