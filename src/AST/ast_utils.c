/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:53:28 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 09:02:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ast.h"

void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	if (node->argv)
		free_strtab(node->argv);
	if (node->filename)
		free(node->filename);
	if (node->left)
		ast_free(node->left);
	if (node->right)
		ast_free(node->right);
	free(node);
}

char	**ft_strdup_array(char **array)
{
	char	**new_array;
	int		i;

	if (!array)
		return (NULL);
	new_array = malloc((ft_str_array_len(array) + 1) * sizeof(char *));
	if (!new_array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_strtab(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

int	ft_str_array_len(char **array)
{
	int	len;

	len = 0;
	while (array[len])
		len++;
	return (len);
}

/*

t_ast	*ast_copy(t_ast *node)
{
	t_ast	*new_node;

	if (!node)
		return (NULL);
	new_node = ast_new(node->type, node->argv ? ft_strdup_array(node->argv) : NULL,
			node->filename ? ft_strdup(node->filename) : NULL, node->redir_type,
			ast_copy(node->left), ast_copy(node->right));
	if (!new_node)
		return (NULL);
	return (new_node);
}

*/