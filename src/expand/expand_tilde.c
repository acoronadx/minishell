/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:46:11 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 02:46:28 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h" // Para t_shell y find_var
#include "env.h"    // Para find_var
#include <stdlib.h> // Para free

/*
** Función auxiliar para expandir la tilde.
** Solo maneja "~" y "~/path".
** Retorna una nueva cadena con la tilde expandida, o NULL en caso de error de malloc.
** Retorna una copia del original si no hay expansión.
*/
char *expand_tilde_internal(const char *str, t_shell *shell)
{
    char *home_path;
    char *new_str;

    if (!str || *str != '~')
        return (ft_strdup(str)); // No empieza con tilde, no expandir

    // Caso de solo "~" o "~/algo"
    // Aseguramos que la tilde no sea parte de un nombre de archivo (ej. "my~file")
    // y que no haya un nombre de usuario después (ej. "~user") para esta implementación simple.
    if (ft_strlen(str) == 1 || str[1] == '/')
    {
        home_path = find_var(shell->env, "HOME"); // Reusa tu find_var
        // Si HOME no está definida o es vacía, o si find_var devuelve "", no expandir
        if (!home_path || *home_path == '\0') 
            return (ft_strdup(str));
        
        if (ft_strlen(str) == 1) // Solo "~"
            return (ft_strdup(home_path));
        else // "~/algo"
        {
            // Concatenar home_path con el resto de la cadena (después de "~")
            new_str = ft_strjoin(home_path, str + 1);
            return (new_str);
        }
    }
    // Para "~user", ft_strdup(str) es el comportamiento actual (no expandir)
    return (ft_strdup(str));
}

/*
** Calcula la longitud del prefijo de tilde que se ha expandido.
** Por ejemplo, si str es "~/foo", devuelve 2 (por "~/")
** si str es "~", devuelve 1 (por "~")
*/
size_t get_tilde_prefix_len(const char *str)
{
    if (!str || *str != '~')
        return (0);
    if (ft_strlen(str) == 1 || str[1] == '/')
        return (1 + (str[1] == '/')); // 1 para '~', 2 para '~/'
    return (1); // Solo '~' si es parte de algo como "~user" (que no expandimos)
}
