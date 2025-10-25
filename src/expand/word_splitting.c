/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:11:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:59:40 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h" // Para t_token y token_new, token_addback

// src/expand/word_splitting.c
static int token_was_quoted_like_bash(const char *s)
{
    t_quote q = NO_QUOTE;
    int i = 0;
    int seen = 0;

    while (s[i])
    {
        if (s[i] == '\\')
        {
            if (q == NO_QUOTE) { if (s[i+1]) i += 2; else i++; continue; }
            if (q == DOUBLE_QUOTE) {
                if (s[i+1] && (s[i+1]=='$' || s[i+1]=='`' || s[i+1]=='"' || s[i+1]=='\\' || s[i+1]=='\n'))
                    i += (s[i+1] ? 2 : 1);
                else i++;
                continue;
            }
            /* q == SINGLE_QUOTE -> '\' literal */
            i++;
            continue;
        }
        if (s[i] == '\'' && q != DOUBLE_QUOTE) { seen = 1; q = (q==SINGLE_QUOTE)?NO_QUOTE:SINGLE_QUOTE; i++; continue; }
        if (s[i] == '"'  && q != SINGLE_QUOTE) { seen = 1; q = (q==DOUBLE_QUOTE)?NO_QUOTE:DOUBLE_QUOTE; i++; continue; }
        i++;
    }
    return seen;
}

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
    return (new_head);
}

void    perform_word_splitting(t_shell *shell)
{
    t_token *current = shell->tokens;
    t_token *prev = NULL;

    while (current)
    {
        t_token *next_original = current->next;

        /* SOLO split si la palabra NO contiene comillado tipo bash */
        if (!token_was_quoted_like_bash(current->value))
        {
            t_token *split_head = split_word_into_tokens(current->value);

            /* expansión no comillada -> cadena vacía => elimina token */
            if (split_head == NULL)
            {
                if (prev) prev->next = next_original;
                else shell->tokens = next_original;
                free(current->value);
                free(current);
                current = next_original;
                continue;
            }

            /* sustituye el token por la lista resultante */
            if (prev) prev->next = split_head;
            else shell->tokens = split_head;

            t_token *last = split_head;
            while (last->next) last = last->next;
            last->next = next_original;

            prev = last;
            free(current->value);
            free(current);
            current = next_original;
        }
        else
        {
            prev = current;
            current = next_original;
        }
    }
}
