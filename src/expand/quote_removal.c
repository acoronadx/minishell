/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:49:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 17:00:24 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"

static char *quote_remove_like_bash(const char *s)
{
    size_t n = ft_strlen(s);
    char *out = malloc(n + 1);
    int i = 0, j = 0;
    t_quote q = NO_QUOTE;

    if (!out) return NULL;

    while (s[i])
    {
        if (s[i] == '\'' && q != DOUBLE_QUOTE) { q = (q==SINGLE_QUOTE)?NO_QUOTE:SINGLE_QUOTE; i++; continue; }
        if (s[i] == '"'  && q != SINGLE_QUOTE) { q = (q==DOUBLE_QUOTE)?NO_QUOTE:DOUBLE_QUOTE; i++; continue; }

        if (s[i] == '\\')
        {
            if (q == SINGLE_QUOTE) { out[j++] = s[i++]; continue; }

            if (q == DOUBLE_QUOTE)
            {
                if (s[i+1] && (s[i+1]=='$' || s[i+1]=='`' || s[i+1]=='"' || s[i+1]=='\\' || s[i+1]=='\n'))
                {
                    if (s[i+1] == '\n') { i += 2; continue; } /* elimina \ + nl */
                    i++; out[j++] = s[i++];                   /* quita '\' y copia escapado */
                    continue;
                }
                /* '\' literal si no escapa algo especial */
                out[j++] = s[i++]; 
                continue;
            }

            /* q == NO_QUOTE */
            if (s[i+1] == '\n') { i += 2; continue; } /* continuación de línea */
            if (s[i+1]) { i++; out[j++] = s[i++]; }   /* quita '\' y copia siguiente */
            else { out[j++] = s[i++]; }               /* '\' final: cópialo */
            continue;
        }

        out[j++] = s[i++];
    }
    out[j] = '\0';
    return out;
}

void    remove_quotes(t_token *tokens)
{
    for (t_token *t = tokens; t; t = t->next)
    {
        char *clean = quote_remove_like_bash(t->value);
        if (!clean) return; /* si quieres, maneja error */
        free(t->value);
        t->value = clean;
        t->quoted = NO_QUOTE;
    }
}
