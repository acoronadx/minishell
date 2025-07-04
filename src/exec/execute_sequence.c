/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_sequence.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:18:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/29 14:18:57 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "ast.h"

int execute_sequence(t_ast *node, t_shell *shell)
{
	execute_ast(node->bin.left, shell);
	return (execute_ast(node->bin.right, shell));
}
