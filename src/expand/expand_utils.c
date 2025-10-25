/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:52:34 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:42:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
** Busca el valor de una variable en el entorno env.
** Devuelve puntero a value o "" si no existe la variable.
*/
char *find_var(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
		{
			if (env->value)
				return (env->value);
			return ("");
		}
		env = env->next;
	}
	return ("");
}

char *get_program_name_str(t_shell *shell)
{
    // Asume que shell->program_name es el nombre del ejecutable del shell
    // Asegúrate de que es una cadena válida y la duplica
    if (!shell->program_name)
        return (ft_strdup("minishell")); // Valor por defecto si no está definido
    return (ft_strdup(shell->program_name));
}
