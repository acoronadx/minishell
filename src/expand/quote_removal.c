/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:49:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:33:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"

static char *quote_remove_like_bash(const char *s)
{
    size_t n = ft_strlen(s);
    char  *out = (char *)malloc(n + 1);
    int    i = 0, j = 0;
    t_quote q = NO_QUOTE;

    if (!out) return NULL;

    while ( s[i] )
    {
        if (q == NO_QUOTE)
        {
            if (s[i] == '\\')
            {
                if (s[i+1] == '\n') { i += 2; continue; }
                if (s[i+1]) { i++; out[j++] = s[i++]; continue; }
                out[j++] = s[i++]; continue; /* '\' final literal */
            }
            if (s[i] == '\'') { q = SINGLE_QUOTE; i++; continue; }
            if (s[i] == '"')  { q = DOUBLE_QUOTE; i++; continue; }
            out[j++] = s[i++]; continue;
        }
        if (q == SINGLE_QUOTE)
        {
            if (s[i] == '\'') { q = NO_QUOTE; i++; continue; }
            out[j++] = s[i++]; continue;
        }
        /* q == DOUBLE_QUOTE */
        if (s[i] == '\\')
        {
            if (s[i+1] == '\n') { i += 2; continue; }
            if (s[i+1] && (s[i+1] == '$' || s[i+1] == '`' || s[i+1] == '"' || s[i+1] == '\\'))
            { i++; out[j++] = s[i++]; continue; }
            out[j++] = s[i++]; continue; /* '\' literal si no escapa especial */
        }
        if (s[i] == '"') { q = NO_QUOTE; i++; continue; }
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
        if (!clean) return; /* Manejo básico de error */
        free(t->value);
        t->value = clean;
        t->quoted = NO_QUOTE;
    }
}
