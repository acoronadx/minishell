/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:11:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 16:11:04 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* IFS por defecto: espacio, tab y newline */
static int is_ifs_space(unsigned char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

/* Divide SOLO palabras (T_WORD) respetando comillas y backslashes */
static t_token *split_word_respecting_quotes(const char *s)
{
    t_token *head = NULL;
    size_t   n = ft_strlen(s);
    char    *buf = malloc(n + 1);
    size_t   i = 0, j;
    t_quote  q;

    if (!buf)
        return NULL;

    /* saltar IFS inicial (no escapado) */
    while (s[i] && is_ifs_space((unsigned char)s[i]))
        i++;

    while (s[i])
    {
        j = 0; q = NO_QUOTE;
        while (s[i])
        {
            char c = s[i];
            if (q == NO_QUOTE)
            {
                if (c == '\\')
                {
                    if (s[i+1] == '\n') { i += 2; continue; }
                    if (s[i+1]) { buf[j++] = '\\'; buf[j++] = s[i+1]; i += 2; continue; }
                    buf[j++] = '\\'; i++; continue;
                }
                if (c == '\'') { q = SINGLE_QUOTE; buf[j++] = c; i++; continue; }
                if (c == '"')  { q = DOUBLE_QUOTE; buf[j++] = c; i++; continue; }
                if (is_ifs_space((unsigned char)c)) break;
                buf[j++] = c; i++; continue;
            }
            else if (q == SINGLE_QUOTE)
            { buf[j++] = c; i++; if (c == '\'') q = NO_QUOTE; continue; }
            else /* DOUBLE_QUOTE */
            {
                if (c == '"') { buf[j++] = c; i++; q = NO_QUOTE; continue; }
                if (c == '\\')
                {
                    if (s[i+1] == '\n') { i += 2; continue; }
                    if (s[i+1]) { buf[j++] = '\\'; buf[j++] = s[i+1]; i += 2; continue; }
                }
                buf[j++] = c; i++; continue;
            }
        }
        if (j > 0)
        {
            char *field = malloc(j + 1);
            if (!field) { free_token_list(head); free(buf); return NULL; }
            ft_memcpy(field, buf, j); field[j] = '\0';
            if (!try_add_token(&head, field, T_WORD, NO_QUOTE))
            { free(field); free_token_list(head); free(buf); return NULL; }
        }
        while (s[i] && is_ifs_space((unsigned char)s[i]))
            i++;
    }
    free(buf);
    return head;
}

void    perform_word_splitting(t_shell *shell)
{
    t_token *current = shell->tokens;
    t_token *prev = NULL;

    while (current)
    {
        t_token *next_original = current->next;

        /* IMPORTANTÍSIMO:
           - SOLO dividimos si es una PALABRA (T_WORD)
           - y si no venía completamente entrecomillada */
        if (current->type == T_WORD && current->quoted == NO_QUOTE)
        {
            t_token *split = split_word_respecting_quotes(current->value);
            if (split == NULL)
            {
                /* Expansión → vacío sin comillas: elimina el token */
                if (prev) prev->next = next_original;
                else shell->tokens = next_original;
                free(current->value);
                free(current);
                current = next_original;
                continue;
            }
            /* Inserta lista nueva en lugar del token original */
            if (prev) prev->next = split;
            else shell->tokens = split;

            t_token *last = split;
            while (last->next) last = last->next;
            last->next = next_original;

            free(current->value);
            free(current);

            prev = last;
            current = next_original;
        }
        else
        {
            prev = current;
            current = next_original;
        }
    }
}
