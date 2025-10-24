/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:49:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 15:51:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"

// strip_quotes_from_value: Elimina las comillas delimitadoras EXTERNAS y procesa backslashes.
// Asume que la expansión de variables ya se realizó.
char *strip_quotes_from_value(const char *value, t_quote original_token_quote_type)
{
    char    *new_value;
    int     i; // Índice para recorrer 'value' (input)
    int     j; // Índice para construir 'new_value' (output)
    t_quote current_internal_quote_state; // Estado interno para rastrear comillas dentro del 'value'

    if (!value)
        return (NULL);

    new_value = ft_calloc(ft_strlen(value) + 1, sizeof(char)); // Memoria generosa
    if (!new_value)
        return (NULL);

    i = 0;
    j = 0;
    current_internal_quote_state = NO_QUOTE; // Inicializamos el estado interno de comillas

    // Flag para saber si hemos pasado las comillas delimitadoras iniciales.
    // Esto es CRUCIAL para distinguir las comillas delimitadoras de las internas.
    bool    found_initial_quote = false; 

    while (value[i])
    {
        // 1. Manejo de Comillas Delimitadoras Externas
        // Estas comillas (la primera y la última si el token original estaba entrecomillado)
        // se eliminan y establecen el 'internal_quote_state'.
        if ((value[i] == '\'' && original_token_quote_type == SINGLE_QUOTE) ||
            (value[i] == '"' && original_token_quote_type == DOUBLE_QUOTE))
        {
            // Solo procesamos la primera comilla delimitadora o la de cierre si estamos en ese estado
            if (!found_initial_quote) // Si es la primera comilla
            {
                current_internal_quote_state = original_token_quote_type;
                found_initial_quote = true;
                i++; // Saltar la comilla (eliminarla)
                continue;
            }
            else if (i == (int)ft_strlen(value) - 1) // Si es la última comilla de la cadena
            {
                current_internal_quote_state = NO_QUOTE; // Cerrar el estado
                i++; // Saltar la comilla (eliminarla)
                continue;
            }
            // Si no es la primera ni la última, se trata como una comilla literal y se copia.
            // Esto es para la " de "comilla\"doble" después de expand_token.
        }

        // 2. Manejo de Backslashes
        // Esta lógica de backslashes es la que debe aplicar después de las comillas externas.
        // Solo las comillas internas (como '\"' ya procesadas a '"') o otros caracteres escapados.
        if (value[i] == '\\')
        {
            if (current_internal_quote_state == SINGLE_QUOTE)
            {
                // Dentro de comillas simples, '\' es literal.
                new_value[j++] = value[i++]; // Copiar '\' tal cual
                continue;
            }
            else // NO_QUOTE o DOUBLE_QUOTE
            {
                i++; // Saltar la barra invertida (la eliminamos)
                if (!value[i]) // Si la '\' es el último carácter de la cadena (ej. "foo\")
                {
                    new_value[j++] = '\\'; // Comportamiento de Bash para '\' al final
                    break; 
                }
                new_value[j++] = value[i++]; // Copiar el carácter escapado
                continue;
            }
        }
        
        // 3. Copiar cualquier otro carácter normal (incluyendo comillas no delimitadoras, como en 'foo"bar' o "comilla\"doble")
        new_value[j++] = value[i++];
    }
    new_value[j] = '\0'; // Null-terminate la cadena resultante
    return (new_value);
}

void    remove_quotes(t_token *tokens)
{
    t_token *current_token;
    char    *stripped_value;

    current_token = tokens;
    while (current_token)
    {
        // Pasa el tipo de quote original del token a la función de stripping
        stripped_value = strip_quotes_from_value(current_token->value, current_token->quoted);
        if (!stripped_value)
        {
            // Manejo de error, quizás liberar todo y retornar un código de error
            return ;
        }
        free(current_token->value);
        current_token->value = stripped_value;
        current_token->quoted = NO_QUOTE; // Después del stripping, el token ya no tiene delimitadores relevantes.
        current_token = current_token->next;
    }
}