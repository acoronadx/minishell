/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 15:44:26 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/* --- Función auxiliar para get_word: calcula el final de una palabra léxica --- */
static int word_end(const char *line, int i)
{
    t_quote current_quote_state = NO_QUOTE; // Rastrea si estamos dentro de comillas (simples/dobles)

    while (line[i])
    {
        // Manejo de Backslashes:
        // Si el caracter actual es un backslash.
        if (line[i] == '\\')
        {
            // Dentro de comillas simples: el backslash es literal, NO escapa nada.
            if (current_quote_state == SINGLE_QUOTE)
            {
                i++; // Consume el '\'
                if (line[i]) // Si hay un caracter después, también lo consume (es literal).
                    i++; 
                continue; 
            }
            // Fuera de comillas simples: el backslash escapa el siguiente carácter.
            // Esto incluye si escapa una comilla: `\"` o `\'` fuera de comillas.
            else if (line[i+1]) // Asegurarse de que hay un caracter para escapar
            {
                i += 2; // Consumimos el backslash y el carácter que escapa
                continue; // Este par de caracteres se incluye y no afecta el estado de las comillas.
            }
            else // `\` es el último caracter. Es literal.
            {
                i++;
                continue;
            }
        }

        // Manejo de Comillas:
        // Si encontramos una comilla (simple o doble) y NO fue escapada por el 'if' anterior
        if (line[i] == '\'')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = SINGLE_QUOTE;
            else if (current_quote_state == SINGLE_QUOTE)
                current_quote_state = NO_QUOTE;
            // Si estaba en DOUBLE_QUOTE, es una comilla literal y no cambia el estado.
            i++;
            continue;
        }
        else if (line[i] == '"')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = DOUBLE_QUOTE;
            else if (current_quote_state == DOUBLE_QUOTE)
                current_quote_state = NO_QUOTE;
            // Si estaba en SINGLE_QUOTE, es una comilla literal y no cambia el estado.
            i++;
            continue;
        }

        // Criterios de terminación de palabra (solo si no estamos dentro de comillas).
        if (current_quote_state == NO_QUOTE)
        {
            if (ft_isspace(line[i]))
                break; // Un espacio termina la palabra
            if (is_operator(line + i, NULL, NULL))
                break; // Un operador termina la palabra
        }
        
        i++; // Avanzar al siguiente carácter si no se manejó antes
    }

    // Si `current_quote_state` no es `NO_QUOTE` al final, indica comillas sin cerrar.
    if (current_quote_state != NO_QUOTE)
        return (-1); // Indica error de comilla sin cerrar

    return (i);
}

// get_word: Extrae una "palabra" léxica de la línea de entrada y determina su tipo de comilla delimitadora.
int get_word(const char *line, int i, t_token **lst)
{
    int     start;
    int     end;      // Fin de la palabra léxica
    char    *word_str; // Cadena de la palabra tal cual, incluyendo comillas y backslashes
    t_quote token_quote_type; // <-- ¡NUEVA VARIABLE PARA EL TIPO DE COMILLA DEL TOKEN!

    start = i;
    end = word_end(line, i); // Determina el final de la palabra

    if (end == -1) // Si word_end indica un error (ej. comillas sin cerrar)
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        free_lexer_list_on_error(lst);
        return (-1);
    }

    int len = end - start;
    if (len == 0)
        return (end);

    word_str = ft_substr(line, start, len);
    if (!word_str)
        return (free_lexer_list_on_error(lst), -1);

    // --- Determinar el tipo de comilla delimitadora del token ---
    token_quote_type = NO_QUOTE;
    if (word_str[0] == '\'')
        token_quote_type = SINGLE_QUOTE;
    else if (word_str[0] == '"')
        token_quote_type = DOUBLE_QUOTE;
    // IMPORTANTE: Esto asume que si el token empieza con una comilla, es la comilla delimitadora.
    // Tu 'word_end' ya se encarga de que la palabra incluya todo hasta la comilla de cierre.
    // Esto es suficiente para la mayoría de los casos simples de delimitación.
    // -------------------------------------------------------------
    if (try_add_token(lst, word_str, T_WORD, token_quote_type) == 0) // <-- ¡USA token_quote_type AQUÍ!
    {
        free(word_str);
        return (-1);
    }

    return (end);
}
