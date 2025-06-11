/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:24:41 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 15:24:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


typedef enum e_node_type
{
	N_COMMAND,
	N_PIPE,
	N_REDIR
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**argv;        // Solo para N_COMMAND
	char			*filename;     // Solo para N_REDIR
	int				redir_type;    // Tipo de redirección
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;