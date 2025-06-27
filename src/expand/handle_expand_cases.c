/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expand_cases.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:08:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 00:15:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
** Busca el valor de una variable en el entorno env.
** Devuelve puntero a value o "" si no existe la variable.
*/
char    *find_var(t_env *env, char *name)
{
    while (env)
    {
        if (ft_strcmp(env->key, name) == 0)
            return (env->value);
        env = env->next;
    }
    return ("");
}


// Devuelve la longitud de la variable expandida sin modificar la cadena 'res'
size_t handle_dollar_len(const char *str, int *i, t_shell *shell)
{
    if (str[*i] == '?')
        return (handle_exit_status_len(shell));
    else if (str[*i] == '0')
        return (ft_strlen(shell->program_name ? shell->program_name : ""));
    else if (str[*i] == '$')
        return (handle_pid_len());
    else // Carácter no válido después de $ (ej. "$#")
    {
        (*i)++; // Avanzar el índice para el carácter después de $
        return (1); // Solo se cuenta el '$' literal (que ya se saltó en el caller) y el carácter siguiente
    }
}


size_t handle_exit_status_len(t_shell *shell)
{
    char    *itoa_exit;
    size_t  len;

    itoa_exit = ft_itoa(shell->exit_status);
    if (!itoa_exit) return ((size_t)-1); // Indicar error de malloc
    len = ft_strlen(itoa_exit);
    free(itoa_exit);
    return (len);
}

size_t handle_pid_len(void)
{
    char    *itoa_pid;
    size_t  len;

    itoa_pid = ft_itoa(getpid());
    if (!itoa_pid) return ((size_t)-1); // Indicar error de malloc
    len = ft_strlen(itoa_pid);
    free(itoa_pid);
    return (len);
}

char *get_program_name_str(t_shell *shell)
{
    // Asume que shell->program_name es el nombre del ejecutable del shell
    // Asegúrate de que es una cadena válida y la duplica
    if (!shell->program_name)
        return (ft_strdup("minishell")); // Valor por defecto si no está definido
    return (ft_strdup(shell->program_name));
}
