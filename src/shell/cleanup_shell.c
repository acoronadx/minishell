/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 03:25:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 00:46:03 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "env.h"

void	free_token_list(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

// Limpia los recursos utilizados en cada iteración del bucle interactivo
// Esto se llama al final de cada iteración del bucle interactivo
// para evitar fugas de memoria y asegurar que cada comando se ejecute con un estado limpio
// y sin residuos de la iteración anterior.
void    cleanup_loop(t_shell *shell)
{
    if (shell->line)
    {
        free(shell->line);
        shell->line = NULL;
    }
    if (shell->tokens)
    {
        free_token_list(shell->tokens);
        shell->tokens = NULL;
    }
}

// Limpia todos los recursos del shell al finalizar
// Se asume que se llama al final de la ejecución del shell, ya sea al salir
// del bucle interactivo o al ejecutar un script o comando único.
// Libera la memoria utilizada por las estructuras del shell y restaura la configuración del terminal.
// También libera el nombre del programa (argv[0]) que se duplicó al inicio.
// Se debe llamar a esta función antes de que el programa termine
// para evitar fugas de memoria y asegurar que todos los recursos se liberen adecuadamente.
void    cleanup_shell(t_shell *shell)
{
    // Libera los recursos dinámicos
    if (shell->env)
    {
        free_env_list(shell->env); // Asumido que libera la lista enlazada del entorno
        shell->env = NULL;
    }
    if (shell->line)
    {
        free(shell->line);
        shell->line = NULL;
    }
    if (shell->tokens)
    {
        free_token_list(shell->tokens);
        shell->tokens = NULL;
    }
    if (shell->program_name)
    {
        free(shell->program_name);
        shell->program_name = NULL;
    }

    // Restaura la configuración del terminal (si aplica)
    restore_vquit(); // Asumido que restaura los manejadores de señales o modos del terminal
}

int	print_exec_error(t_shell *shell, const char *cmd, int err_code)
{
	int	status;

	if (err_code == ENOENT)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", cmd);
		status = 127;
	}
	else if (err_code == EACCES)
	{
		if (ft_strchr(cmd, '/'))
			ft_dprintf(2, "%s: Permission denied\n", cmd);
		else
			ft_dprintf(2, "minishell: %s: permission denied\n", cmd);
		status = 126;
	}
	else if (err_code == EISDIR)
	{
		ft_dprintf(2, "minishell: %s: is a directory\n", cmd);
		status = 126;
	}
	else
	{
		ft_dprintf(2, "minishell: %s: execution error\n", cmd);
		status = 1;
	}
	shell->exit_status = status;
	return (status);
}
