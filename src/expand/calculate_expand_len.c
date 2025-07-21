/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_expand_len.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:16:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/21 14:13:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
static size_t	handle_tilde_len(const char *str, int *i, t_shell *shell)
{
	char	*tilde_exp;
	size_t	len;

	tilde_exp = expand_tilde_internal(str + *i, shell);
	if (!tilde_exp)
		return ((size_t)-1);
	len = ft_strlen(tilde_exp);
	*i += get_tilde_prefix_len(str + *i);
	free(tilde_exp);
	return (len);
}

static size_t	handle_regular_char(const char *str, int *i)
{
	(void)str;
	(*i)++;
	return (1);
}
*/
size_t	handle_dollar_len(const char *str, int *i, t_shell *shell)
{
	char	*expanded;
	size_t	len;

	(*i)++; // saltar el $
	expanded = expand_value(str, i, shell); // esta función debe avanzar i correctamente
	if (!expanded)
		return ((size_t)-1);
	len = ft_strlen(expanded);
	free(expanded);
	return (len);
}

size_t  calculate_expanded_len(const char *str, t_shell *shell)
{
    size_t  total_len;
    int     i;
    int     temp_i; // Usamos un temporal para no afectar el 'i' principal de la iteración
    t_quote current_quote_state;
    size_t  len_to_add;

    total_len = 0;
    i = 0;
    current_quote_state = NO_QUOTE;

    while (str[i])
    {
        temp_i = i; // Guardamos el índice actual para no modificar 'i' directamente
        len_to_add = 0;

        // 1. Manejo de Comillas (contamos la comilla como 1 caracter)
        if (str[temp_i] == '\'')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = SINGLE_QUOTE;
            else if (current_quote_state == SINGLE_QUOTE)
                current_quote_state = NO_QUOTE;
            len_to_add = 1;
            temp_i++;
        }
        else if (str[temp_i] == '"')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = DOUBLE_QUOTE;
            else if (current_quote_state == DOUBLE_QUOTE)
                current_quote_state = NO_QUOTE;
            len_to_add = 1;
            temp_i++;
        }
        // 2. Expansión (solo si no estamos dentro de comillas simples)
        else if (current_quote_state != SINGLE_QUOTE)
        {
            // Manejo de Backslash antes de $ dentro de comillas dobles
            if (str[temp_i] == '\\' && str[temp_i + 1] && current_quote_state == DOUBLE_QUOTE)
            {
                if (str[temp_i + 1] == '$' || str[temp_i + 1] == '"' || str[temp_i + 1] == '\\')
                {
                    len_to_add = 1; // El backslash se elimina, solo cuenta el caracter escapado
                    temp_i += 2;
                }
                else
                { // No es un caracter especial escapado, solo copia el backslash y el caracter
                    len_to_add = 2;
                    temp_i += 2;
                }
            }
            // Manejo de $
            else if (str[temp_i] == '$')
            {
                // handle_dollar_len ya avanza temp_i
                len_to_add = handle_dollar_len(str, &temp_i, shell);
                if (len_to_add == (size_t)-1) return ((size_t)-1);
            }
            // Manejo de Tilde
            else if (str[temp_i] == '~' && (temp_i == 0 || ft_isspace(str[temp_i - 1]) || str[temp_i - 1] == '='))
            {
                char *tilde_exp = expand_tilde_internal(str + temp_i, shell);
                if (!tilde_exp) return ((size_t)-1);
                len_to_add = ft_strlen(tilde_exp);
                temp_i += get_tilde_prefix_len(str + temp_i);
                free(tilde_exp);
            }
            // Backslash fuera de comillas dobles
            else if (str[temp_i] == '\\' && str[temp_i + 1] && current_quote_state == NO_QUOTE)
            {
                len_to_add = 1; // El backslash se elimina
                temp_i += 2;
            }
            // Caracter normal
            else
            {
                len_to_add = 1;
                temp_i++;
            }
        }
        // Si estamos en comillas simples, o es un caracter normal no expandible
        else
        {
            len_to_add = 1;
            temp_i++;
        }
        
        total_len += len_to_add;
        i = temp_i; // Actualiza el índice principal
    }
    return (total_len);
}
