/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_expand_len.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:16:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:32:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

size_t  handle_dollar_len(const char *str, int *i, t_shell *shell)
{
    char    *expanded;
    size_t  len;

    (*i)++; /* saltar '$' */
    expanded = expand_value(str, i, shell); /* avanza *i tras el nombre */
    if (!expanded)
        return (size_t)-1;
    len = ft_strlen(expanded);
    free(expanded);
    return len;
}

size_t  calculate_expanded_len(const char *str, t_shell *shell)
{
    size_t  total = 0;
    int     i = 0;
    t_quote q = NO_QUOTE;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            if (q == NO_QUOTE) q = SINGLE_QUOTE;
            else if (q == SINGLE_QUOTE) q = NO_QUOTE;
            total += 1; i++; continue;
        }
        if (str[i] == '"')
        {
            if (q == NO_QUOTE) q = DOUBLE_QUOTE;
            else if (q == DOUBLE_QUOTE) q = NO_QUOTE;
            total += 1; i++; continue;
        }

        if (str[i] == '\\')
        {
            if (q == SINGLE_QUOTE) { total += 1; i += 1; continue; }
            if (q == DOUBLE_QUOTE)
            {
                if (str[i+1] == '\n') { i += 2; continue; }
                if (str[i+1]) { total += 2; i += 2; continue; } /* '\'+char */
                total += 1; i += 1; continue; /* '\' final */
            }
            /* NO_QUOTE */
            if (str[i+1] == '\n') { i += 2; continue; }
            if (str[i+1] == '$') { total += 1; i += 2; continue; } /* \$ -> '$' */
            total += 1; i += 1; continue; /* preservamos '\' */
        }

        if (q != SINGLE_QUOTE && str[i] == '$')
        {
            size_t add = handle_dollar_len(str, &i, shell);
            if (add == (size_t)-1) return (size_t)-1;
            total += add;
            continue;
        }

        if (str[i] == '~' && (i == 0 || ft_isspace((unsigned char)str[i-1]) || str[i-1] == '='))
        {
            char *t = expand_tilde_internal(str + i, shell);
            if (!t) return (size_t)-1;
            total += ft_strlen(t);
            i += get_tilde_prefix_len(str + i);
            free(t);
            continue;
        }

        total += 1; i++;
    }
    return total;
}
