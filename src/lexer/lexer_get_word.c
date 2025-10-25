/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 14:41:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Avanza hasta el fin de una "palabra" al estilo bash:
** - Respeta comillas (simples/dobles) y sus reglas de escapes.
** - Fuera de comillas, '\' escapa siguiente char; '\\n' se elimina (continuación).
** - Un operador reconocido TERMINA la palabra si no estamos dentro de comillas.
** - Espacios no comillados también terminan palabra.
*/

static int word_end(const char *s, int i)
{
    t_quote q = NO_QUOTE;

    while (s[i])
    {
        char c = s[i];

        if (q == NO_QUOTE)
        {
            /* backslash fuera de comillas: escapa un char; "\<nl>" continúa línea. :contentReference[oaicite:8]{index=8} */
            if (c == '\\')
            {
                if (s[i + 1] == '\n') { i += 2; continue; }
                i += (s[i + 1] ? 2 : 1);
                continue;
            }
            /* comillas */
            if (c == '\'') { q = SINGLE_QUOTE; i++; continue; }
            if (c == '"')  { q = DOUBLE_QUOTE; i++; continue; }

            if (ft_isspace(c))
                break;

            /* operador -> fin de palabra */
            if (is_operator(s + i, NULL, NULL))
                break;

            i++;
        }
        else if (q == SINGLE_QUOTE)
        {
            /* dentro de '...': todo literal hasta siguiente ' (sin escapes). :contentReference[oaicite:9]{index=9} */
            if (c == '\'') q = NO_QUOTE;
            i++;
        }
        else /* DOUBLE_QUOTE */
        {
            /* dentro de "...": \ sólo escapa $, `, ", \ y newline. :contentReference[oaicite:10]{index=10} */
            if (c == '\\' && (s[i + 1] == '$' || s[i + 1] == '`'
                || s[i + 1] == '"' || s[i + 1] == '\\' || s[i + 1] == '\n'))
            {
                i += (s[i + 1] ? 2 : 1);
                continue;
            }
            if (c == '"') q = NO_QUOTE;
            i++;
        }
    }
    if (q != NO_QUOTE)
        return -1; /* comillas sin cerrar -> error sintaxis */
    return i;
}

int get_word(const char *line, int i, t_token **lst)
{
    int     start = i;
    int     end = word_end(line, i);
    char    *word;
    t_quote qtype = NO_QUOTE;

    if (end < 0)
    {
        fprintf(stderr, "minishell: syntax error: unclosed quote\n");
        free_lexer_list_on_error(lst);
        return -1;
    }
    if (end == start)
        return end;

    word = ft_substr(line, start, end - start);
    if (!word)
        return (free_lexer_list_on_error(lst), -1);

    /* Determina el comillado principal del token (si empieza por comilla) */
    if (word[0] == '\'') qtype = SINGLE_QUOTE;
    else if (word[0] == '"') qtype = DOUBLE_QUOTE;

    if (!try_add_token(lst, word, T_WORD, qtype))
    {
        free(word);
        return -1;
    }
    return end;
}
