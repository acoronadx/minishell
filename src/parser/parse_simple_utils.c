/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:05:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 14:52:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

int	is_lparen(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == T_LPAREN);
}
