/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:42:49 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 01:07:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_and_execute(t_shell *shell, char *line)
{
	// 1. Tokeniza la línea (con comillas, pipes, redirecciones…)
	parse_line(shell, line);
	free(line);

	if (!shell->tokens)
		return ;

	// 2. Expande variables en tokens marcados con .expand
	expand_tokens(shell);

	// 3. Parse tokens → comandos
	t_cmd *cmds = parse_commands(shell->tokens);

	// 4. Ejecutar los comandos
	// execute_commands(shell, cmds); // <-- esta función la vas a implementar después

	// 5. Liberar recursos
	free_cmds(cmds);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
}
