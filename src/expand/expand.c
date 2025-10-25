/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 00:59:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h" // Asegúrate de que esta cabecera tenga las declaraciones necesarias (t_quote, etc.)
#include "env.h"    // Para t_shell y manejo de entorno


static int dq_escapable(char c) { return (c=='$' || c=='`' || c=='"' || c=='\\' || c=='\n'); }

char *expand_token(const char *str, t_shell *shell)
{
    if (!str) return NULL;

    size_t need = calculate_expanded_len(str, shell); /* evita overflows con $PATH */
    if (need == (size_t)-1) return NULL;

    char *res = malloc(need + 1);
    if (!res) return NULL;

    int i = 0, j = 0;
    t_quote q = NO_QUOTE;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            if (q == NO_QUOTE) q = SINGLE_QUOTE;
            else if (q == SINGLE_QUOTE) q = NO_QUOTE;
            res[j++] = str[i++];                   /* quote se eliminará en remove_quotes */
            continue;
        }
        if (str[i] == '"')
        {
            if (q == NO_QUOTE) q = DOUBLE_QUOTE;
            else if (q == DOUBLE_QUOTE) q = NO_QUOTE;
            res[j++] = str[i++];
            continue;
        }

        if (str[i] == '\\')
        {
            if (q == SINGLE_QUOTE) { res[j++] = str[i++]; continue; }

            if (q == DOUBLE_QUOTE)
            {
                if (str[i+1] && dq_escapable(str[i+1]))
                {
                    if (str[i+1] == '\n') { i += 2; continue; }     /* quita \+nl */
                    i++; res[j++] = str[i++];                       /* copia char escapado */
                    continue;
                }
                res[j++] = str[i++];                                /* '\' literal */
                continue;
            }

            /* NO_QUOTE */
            if (str[i+1] == '\n') { i += 2; continue; }
            if (str[i+1]) { i++; res[j++] = str[i++]; }             /* quita '\' y copia 1 */
            else { res[j++] = str[i++]; }                           /* '\' final literal */
            continue;
        }

        if (q != SINGLE_QUOTE && str[i] == '$')
        {
            int added = handle_dollar(str, &i, res, j, shell);      /* avanza i internamente */
            if (added < 0) { free(res); return NULL; }
            j += added;
            continue;
        }

        if (str[i] == '~' && (i == 0 || ft_isspace((unsigned char)str[i-1]) || str[i-1] == '='))
        {
            char *t = expand_tilde_internal(str + i, shell);
            if (!t) { free(res); return NULL; }
            ft_strcpy(res + j, t);
            j += ft_strlen(t);
            i += get_tilde_prefix_len(str + i);
            free(t);
            continue;
        }

        res[j++] = str[i++];
    }
    res[j] = '\0';
    return res;
}

// src/expand/expand.c
int expand_variables(t_shell *shell)
{
    t_token *t = shell->tokens;

    while (t)
    {
        char *expanded = expand_token(t->value, shell);
        if (!expanded)
            return -1;
        free(t->value);
        t->value = expanded;
        t = t->next;
    }

    /* Bash: expansiones -> word splitting -> quote removal */
    perform_word_splitting(shell);   // <-- AHORA SÍ
    remove_quotes(shell->tokens);
    return 0;
}
