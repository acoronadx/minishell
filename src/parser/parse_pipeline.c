/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:38:38 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 03:02:40 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"

t_ast	*parse_pipeline(t_token **cur)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	left = parse_command_and_redirections(cur);
	if (!left)
		return (NULL);
	while (*cur && (*cur)->type == T_PIPE)
	{
		next_token(cur);
		if (!*cur)
		{
			ft_dprintf(2,
				"minishell: syntax error near unexpected token 'newline'\n");
			free_ast(left);
			return (NULL);
		}
		right = parse_command_and_redirections(cur);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		node = ast_new_binary(N_PIPE, left, right);
		if (!node)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		left = node;
	}
	return (left);
}
