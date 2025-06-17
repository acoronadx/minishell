/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:09:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 09:10:13 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

static int	check_pipe_node(t_ast *node)
{
	if (!node->left || !node->right)
		return (0);
	if (!check_syntax(node->left))
		return (0);
	if (!check_syntax(node->right))
		return (0);
	return (1);
}

static int	check_redir_node(t_ast *node)
{
	if (!node->filename || !node->left)
		return (0);
	if (!check_syntax(node->left))
		return (0);
	return (1);
}

static int	check_binary_node(t_ast *node)
{
	if (!node->left || !node->right)
		return (0);
	if (!check_syntax(node->left))
		return (0);
	if (!check_syntax(node->right))
		return (0);
	return (1);
}

static int	check_subshell_node(t_ast *node)
{
	if (!node->left)
		return (0);
	if (!check_syntax(node->left))
		return (0);
	return (1);
}

int	check_syntax(t_ast *node)
{
	if (!node)
		return (0);
	if (node->type == N_PIPE)
		return (check_pipe_node(node));
	if (node->type == N_REDIR)
		return (check_redir_node(node));
	if (node->type == N_AND || node->type == N_OR
		|| node->type == N_SEQUENCE || node->type == N_BACKGROUND)
		return (check_binary_node(node));
	if (node->type == N_SUBSHELL)
		return (check_subshell_node(node));
	return (1);
}
