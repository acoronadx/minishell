/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_and_redirection.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 06:56:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 15:18:17 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_command_and_redirections(t_token **cur)
{
	if (*cur && (*cur)->type == T_LPAREN)
		return (parse_subshell(cur));
	if (*cur && ((*cur)->type == T_WORD || is_redirection(*cur)))
		return (parse_simple_command(cur));
	ft_dprintf(2, "minishell: syntax error near unexpected token '%s'\n",
		*cur ? (*cur)->value : "newline");
	return (NULL);
}
