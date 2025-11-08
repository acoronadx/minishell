/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:53:28 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 17:06:20 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*ast_new_command(char **argv, t_ast *redirections)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = N_COMMAND;
	node->next = NULL;
	node->cmd.argv = argv;
	node->cmd.redirections = redirections;
	return (node);
}

t_ast	*ast_new_redir(char *filename, char *delimiter,
		t_redir_type type, int redir_fd)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		free(filename);
		free(delimiter);
		return (NULL);
	}
	node->type = N_REDIR;
	node->next = NULL;
	node->redir.filename = filename;
	node->redir.delimiter = delimiter;
	node->redir.redir_type = type;
	node->redir.redir_fd = redir_fd;
	node->redir.heredoc_fd = -1;
	node->redir.heredoc_quoted = 0;
	return (node);
}

t_ast	*ast_new_binary(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->next = NULL;
	node->bin.left = left;
	node->bin.right = right;
	return (node);
}

t_ast	*ast_new_subshell(t_ast *child, t_ast *redirections)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = N_SUBSHELL;
	node->next = NULL;
	node->subshell.child = child;
	node->subshell.redirections = redirections;
	return (node);
}

t_ast	*create_command_node(char **argv, t_ast *redir_list_head)
{
	return (ast_new_command(argv, redir_list_head));
}
