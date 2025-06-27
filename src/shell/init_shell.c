/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 03:25:09 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/27 19:56:47 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

int	init_shell_name(t_shell *shell, char **argv)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->exit_status = 0;
	shell->program_name = ft_strdup(argv[0]);
	if (!shell->program_name)
	{
		ft_dprintf(2, "minishell: Error fatal: No se pudo asignar el nombre del programa.\n");
		return (1);
	}
	return (0);
}

int init_env(t_shell *shell, char **envp)
{
    shell->env = init_env_list(envp);
    if (!shell->env)
    {
        ft_dprintf(2, "minishell: Error fatal: No se pudo inicializar el entorno.\n");
        return (1);
    }
    return (0);
}
