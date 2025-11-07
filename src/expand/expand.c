/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 18:57:35 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ===== variantes que respetan is_hdoc_delim ===== */

static void remove_quotes_skip(t_token *tokens)
{
    for (t_token *t = tokens; t; t = t->next)
    {
        if (t->is_hdoc_delim)          /* ¡no tocar! */
            continue;

        char *clean = quote_remove_like_bash(t->value);
        if (!clean)
            return; /* si quieres, maneja error global */
        free(t->value);
        t->value = clean;
        t->quoted = NO_QUOTE;
    }
}

/* Reutiliza tu split_word_respecting_quotes existente */
extern t_token *split_word_respecting_quotes(const char *s);

static void perform_word_splitting_skip(t_shell *sh)
{
    t_token *cur = sh->tokens, *prev = NULL;

    while (cur)
    {
        t_token *next = cur->next;

        if (cur->type == T_WORD && cur->quoted == NO_QUOTE && !cur->is_hdoc_delim)
        {
            t_token *split = split_word_respecting_quotes(cur->value);
            if (split == NULL)
            {
                /* se quedó en 0 campos → elimina token */
                if (prev) prev->next = next; else sh->tokens = next;
                free(cur->value);
                free(cur);
                cur = next;
                continue;
            }
            /* Inserta la lista split en lugar del original */
            if (prev) prev->next = split; else sh->tokens = split;
            t_token *last = split; while (last->next) last = last->next;
            last->next = next;

            free(cur->value);
            free(cur);
            prev = last;
        }
        else
        {
            prev = cur;
        }
        cur = next;
    }
}

int expand_variables(t_shell *shell)
{
    t_token *t = shell->tokens;

    while (t)
    {
        if (!t->is_hdoc_delim) /* <-- no tocar el delimitador de << */
        {
            char *expanded = expand_token(t->value, shell);
            if (!expanded)
            {
                /* deja consistente el token y aborta */
                free(t->value);
                t->value = NULL;
                return -1;
            }
            free(t->value);
            t->value = expanded;
        }
        t = t->next;
    }
    perform_word_splitting_skip(shell);
    remove_quotes_skip(shell->tokens);
    return 0;
}
