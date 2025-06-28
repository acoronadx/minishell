/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:05:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 04:01:55 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"   
#include "lexer.h"

int	is_lparen(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == T_LPAREN);
}

void	next_token(t_token **cur)
{
	if (cur && *cur)
		*cur = (*cur)->next;
}

