/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_expand_len.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:16:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 00:16:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

// --- Funciones auxiliares para el cálculo de longitud ---

size_t calculate_expanded_len(const char *str, t_shell *shell)
{
    size_t  len = 0;
    int     i = 0;
    char    *tilde_expanded;
    size_t  tilde_len;

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++; // Saltar el '$'
            size_t dollar_len = handle_dollar_len(str, &i, shell);
            if (dollar_len == (size_t)-1) // Indicar error de malloc si itoa falla aquí
                return ((size_t)-1);
            len += dollar_len;
        }
        else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1]))) // Posible tilde al inicio o después de espacio
        {
            // Para calcular la longitud de la tilde expandida, la expandimos temporalmente
            tilde_expanded = expand_tilde_internal(str + i, shell);
            if (!tilde_expanded) return ((size_t)-1); // Error de malloc en ft_strdup o ft_strjoin
            tilde_len = ft_strlen(tilde_expanded);
            len += tilde_len;
            i += get_tilde_prefix_len(str + i); // Avanzar 'i' la longitud de la tilde reconocida
            free(tilde_expanded); // Liberar la expansión temporal
        }
        else
        {
            len++;
            i++;
        }
    }
    return (len);
}
