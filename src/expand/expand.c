/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:34:38 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

char *expand_token(const char *str, t_shell *shell)
{
    if (!str) return NULL;

    /* Pre-calcular tamaño para evitar overflows (p.ej. $PATH muy largo) */
    size_t need = calculate_expanded_len(str, shell);
    if (need == (size_t)-1)
        return NULL;

    char *res = (char *)malloc(need + 1);
    if (!res) return NULL;

    int i = 0, j = 0;
    t_quote q = NO_QUOTE;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            if (q == NO_QUOTE) q = SINGLE_QUOTE;
            else if (q == SINGLE_QUOTE) q = NO_QUOTE;
            res[j++] = str[i++];                 /* se quitará en remove_quotes */
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
            if (q == SINGLE_QUOTE)
            {
                /* en '...' la barra es literal */
                res[j++] = '\\'; i++; continue;
            }
            if (q == DOUBLE_QUOTE)
            {
                if (str[i+1] == '\n') { i += 2; continue; } /* "\<nl>" desaparece */
                if (str[i+1])
                {
                    /* conservamos '\' + char; quote_removal decidirá si quitar '\' */
                    res[j++] = '\\';
                    res[j++] = str[i+1];
                    i += 2;
                    continue;
                }
                res[j++] = '\\'; i++; continue;            /* '\' final */
            }
            /* NO_QUOTE */
            if (str[i+1] == '\n') { i += 2; continue; }    /* continuación de línea */
            if (str[i+1] == '$') { i++; res[j++] = '$'; i++; continue; } /* \$ -> '$' literal */
            res[j++] = '\\'; i++; continue;                /* preservamos '\' */
        }

        if (q != SINGLE_QUOTE && str[i] == '$')
        {
            int added = handle_dollar(str, &i, res, j, shell); /* avanza i internamente */
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
    /* Orden Bash: expansiones -> word splitting -> quote removal */
    perform_word_splitting(shell);
    remove_quotes(shell->tokens);
    return 0;
}
