/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:11:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/21 15:17:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h" // Para t_token y token_new, token_addback

// Helper para crear una nueva lista de tokens a partir de una cadena con word splitting
t_token *split_word_into_tokens(char *word) // <-- Eliminado t_token_type type
{
    t_token *new_head = NULL;
    char    *start;
    char    *end;
    char    *sub_word;

    if (!word)
        return NULL;

    start = word;

    while (*start)
    {
        // 1. Saltar los delimitadores de campo (espacios)
        while (*start && ft_isspace(*start))
        {
            start++;
        }
        if (!*start) // Si hemos llegado al final de la cadena (solo había espacios)
            break;

        // 2. Encontrar el final del "campo" (la siguiente secuencia de caracteres no-espacio)
        end = start;
        while (*end && !ft_isspace(*end))
        {
            end++;
        }
        
        // 3. Crear el sub-token para este campo
        if (end > start)
        {
            sub_word = ft_substr(start, 0, end - start);
            if (!sub_word)
            {
                free_token_list(new_head);
                return NULL;
            }
            // Añadir el nuevo token a la lista. El tipo es siempre T_WORD aquí.
            if (!try_add_token(&new_head, sub_word, T_WORD, NO_QUOTE)) 
            {
                free(sub_word);
                free_token_list(new_head);
                return NULL;
            }
        }
        // 4. Mover 'start' al final del campo actual para la siguiente iteración
        start = end;
    }
    return new_head;
}

// Función principal de word splitting
void    perform_word_splitting(t_shell *shell)
{
    t_token *current;
    t_token *prev = NULL;
    t_token *next_original;
    t_token *split_tokens_head;

    current = shell->tokens;
    while (current)
    {
        next_original = current->next; // Guardar el siguiente token original

        // Solo realizamos word splitting si el token NO ESTABA entre comillas.
        // Los valores entrecomillados preservan sus espacios y no se dividen.
        // Si tu lexer no está marcando `quoted`, aquí tendrás que ser más astuto.
        // Asumo que current->quoted indica el estado de la palabra original.
        if (current->quoted == NO_QUOTE) // O un tipo que indica que no fue entrecomillado
        {
            split_tokens_head = split_word_into_tokens(current->value);
            if (split_tokens_head == NULL)
            {
                // Si la expansión resultó en una cadena vacía (como `$a`),
                // split_word_into_tokens podría devolver NULL.
                // En ese caso, simplemente "eliminamos" este token de la lista.
                if (prev)
                    prev->next = next_original;
                else
                    shell->tokens = next_original;
                free(current->value);
                free(current);
                current = next_original;
                continue; // Pasa al siguiente token
            }
            
            // Reemplazar el token original con la lista de tokens divididos
            if (prev)
                prev->next = split_tokens_head;
            else
                shell->tokens = split_tokens_head;
            
            // Conectar el final de la nueva lista dividida al resto de la lista original
            t_token *last_split = split_tokens_head;
            while (last_split->next)
                last_split = last_split->next;
            last_split->next = next_original;

            // Actualizar 'prev' y 'current' para la siguiente iteración
            prev = last_split;
            free(current->value); // Liberar el valor original
            free(current);        // Liberar el nodo original
            current = next_original; // Continuar desde el siguiente token original
        }
        else // Si el token estaba entre comillas, no se aplica word splitting
        {
            prev = current;
            current = current->next;
        }
    }
}
