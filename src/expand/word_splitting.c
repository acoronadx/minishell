/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:11:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:46:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"   // try_add_token, t_token, free_token_list
#include "expand.h"  // t_quote

/* Mini IFS: espacio, tab y newline (igual que bash por defecto) */
static int is_ifs_space(unsigned char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

/*
 * Divide un "word" en campos respetando:
 *  - comillas simples y dobles (los espacios entrecomillados NO dividen),
 *  - barras invertidas: fuera de comillas, '\' protege el siguiente char
 *    (incluido el espacio), por lo que NO debe dividir ahí.
 *  - dentro de "..." dejamos '\' + char tal cual (quote_removal se encarga).
 *  - dentro de '...' la '\' es literal.
 *
 * NO elimina comillas ni backslashes: eso lo hará remove_quotes() después,
 * como dicta el orden de expansiones de Bash (expansiones -> word splitting -> quote removal). 
 */
static t_token *split_word_respecting_quotes(const char *s)
{
    t_token *head = NULL;
    size_t   n = ft_strlen(s);
    char    *buf = malloc(n + 1);   /* suficiente: nunca generamos algo más largo */
    size_t   i = 0, j;
    t_quote  q;

    if (!buf)
        return NULL;

    /* Saltar IFS inicial sin escapado (si lo hubiera, ya vendría como '\ ' en s) */
    while (s[i] && is_ifs_space((unsigned char)s[i]))
        i++;

    while (s[i])
    {
        j = 0;
        q = NO_QUOTE;

        while (s[i])
        {
            char c = s[i];

            if (q == NO_QUOTE)
            {
                if (c == '\\')
                {
                    /* \ + newline => continuación de línea: elimina ambos */
                    if (s[i+1] == '\n') { i += 2; continue; }
                    /* protegemos el siguiente char (incluido un espacio) */
                    if (s[i+1]) { buf[j++] = '\\'; buf[j++] = s[i+1]; i += 2; continue; }
                    /* '\' final */
                    buf[j++] = '\\'; i++; continue;
                }
                if (c == '\'') { q = SINGLE_QUOTE; buf[j++] = c; i++; continue; }
                if (c == '"')  { q = DOUBLE_QUOTE; buf[j++] = c; i++; continue; }

                /* delimitador real: espacio NO escapado y NO entrecomillado */
                if (is_ifs_space((unsigned char)c))
                    break;

                buf[j++] = c; i++; continue;
            }
            else if (q == SINGLE_QUOTE)
            {
                buf[j++] = c; i++;
                if (c == '\'') q = NO_QUOTE;
                continue;
            }
            else /* q == DOUBLE_QUOTE */
            {
                if (c == '"') { buf[j++] = c; i++; q = NO_QUOTE; continue; }
                if (c == '\\')
                {
                    /* dentro de "..." dejamos '\' + char; quote_removal decidirá */
                    if (s[i+1] == '\n') { i += 2; continue; }
                    if (s[i+1]) { buf[j++] = '\\'; buf[j++] = s[i+1]; i += 2; continue; }
                    buf[j++] = '\\'; i++; continue;
                }
                buf[j++] = c; i++; continue;
            }
        }

        /* cerramos campo si hemos acumulado algo */
        if (j > 0)
        {
            char *field = malloc(j + 1);
            if (!field) { free_token_list(head); free(buf); return NULL; }
            ft_memcpy(field, buf, j);
            field[j] = '\0';

            if (!try_add_token(&head, field, T_WORD, NO_QUOTE))
            {
                free(field);
                free_token_list(head);
                free(buf);
                return NULL;
            }
        }

        /* saltar separadores consecutivos */
        while (s[i] && is_ifs_space((unsigned char)s[i]))
            i++;
    }

    free(buf);
    return head; /* puede ser NULL si no se generó ningún campo */
}

/* Función principal de word splitting */
void    perform_word_splitting(t_shell *shell)
{
    t_token *current = shell->tokens;
    t_token *prev = NULL;

    while (current)
    {
        t_token *next_original = current->next;

        /* Si el token original venía entrecomillado completo, NO dividir */
        if (current->quoted == NO_QUOTE)
        {
            t_token *split = split_word_respecting_quotes(current->value);
            if (split == NULL)
            {
                /* Eliminar el token (p.ej., expansión vacía) */
                if (prev)
                    prev->next = next_original;
                else
                    shell->tokens = next_original;
                free(current->value);
                free(current);
                current = next_original;
                continue;
            }

            /* Conectar nueva lista en lugar del token actual */
            if (prev)
                prev->next = split;
            else
                shell->tokens = split;

            /* Ir al final de la lista recién creada */
            t_token *last = split;
            while (last->next)
                last = last->next;

            last->next = next_original;

            /* liberar el nodo antiguo */
            free(current->value);
            free(current);

            prev = last;
            current = next_original;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}
