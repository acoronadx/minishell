/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 01:29:10 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

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
            j += new_j;  // Aquí sumamos, no asignamos
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

int expand_variables(t_shell *shell)
{
    t_token *current_token;
    char    *expanded_value;

    if (!shell->tokens)
        return (0);

    current_token = shell->tokens;
    while (current_token)
    {
        // Solo expandimos si el token no está en comillas simples
        if (current_token->quoted != SINGLE_QUOTE)
        {
            expanded_value = expand_token(current_token->value, shell);
            if (!expanded_value)
            {
                free_token_list(shell->tokens);
                shell->tokens = NULL;
                return (-1);
            }
            free(current_token->value);
            current_token->value = expanded_value;
        }
        current_token = current_token->next;
    }
    return (0);
}


