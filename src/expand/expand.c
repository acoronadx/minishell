/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 00:17:01 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
** Expande las variables presentes en el string str usando el entorno shell.
** Devuelve una nueva cadena con las variables expandidas.
** Retorna NULL en caso de error de asignación de memoria.
*/
char    *expand_token(const char *str, t_shell *shell)
{
    int     i;
    int     j;
    char    *res;
    size_t  required_len;

    // 1. Calcular la longitud necesaria para la cadena expandida
    required_len = calculate_expanded_len(str, shell);
    if (required_len == (size_t)-1)
        return (NULL);

    // 2. Asignar memoria exacta para la cadena expandida
    res = ft_calloc(required_len + 1, 1);
    if (!res)
        return (NULL);

    // 3. Construir la cadena expandida
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '$')
        {
            int new_j = handle_dollar(str, &i, res, j, shell);
            if (new_j == -1)
            {
                free(res);
                return (NULL);
            }
            j = new_j;
        }
        else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1])))
        {
            char *tilde_expanded = expand_tilde_internal(str + i, shell);
            if (!tilde_expanded)
            {
                free(res);
                return (NULL);
            }
            ft_strcpy(res + j, tilde_expanded);
            j += ft_strlen(tilde_expanded);
            i += get_tilde_prefix_len(str + i);
            free(tilde_expanded);
        }
        else
        {
            res[j] = str[i];
            j++;
            i++;
        }
    }
    res[j] = '\0';
    return (res);
}


/*
** Expande todas las variables de los tokens en la lista de shell.
** Modifica la lista in-situ.
** Retorna 0 en caso de éxito, -1 en caso de error de memoria.
*/
int expand_variables(t_shell *shell)
{
    t_token *current_token;
    char    *expanded_value;

    if (!shell->tokens)
        return (0); // No hay tokens, éxito.

    current_token = shell->tokens;
    while (current_token)
    {
        // Solo expandimos si el token no está en comillas simples
        if (current_token->quoted != SINGLE_QUOTE)
        {
            expanded_value = expand_token(current_token->value, shell);
            if (!expanded_value)
            {
                // Error crítico: liberar todos los tokens ya procesados/originales.
                free_token_list(shell->tokens);
                shell->tokens = NULL; // Importante: la lista ya no es válida
                return (-1); // Indicar que la expansión falló
            }
            free(current_token->value); // Liberar el valor original
            current_token->value = expanded_value; // Asignar el nuevo valor expandido
        }
        current_token = current_token->next;
    }
    return (0); // Éxito
}


