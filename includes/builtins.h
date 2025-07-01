/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:31:23 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/29 15:41:02 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
#define BUILTINS_H

#include "minishell.h"
#include "ast.h"
#include "exec.h"

int		is_builtin(const char *cmd);
int		run_builtin(t_ast *node, t_shell *shell);

#endif
