/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_expand_len.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:16:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 00:58:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
static size_t	handle_tilde_len(const char *str, int *i, t_shell *shell)
{
	char	*tilde_exp;
	size_t	len;

	tilde_exp = expand_tilde_internal(str + *i, shell);
	if (!tilde_exp)
		return ((size_t)-1);
	len = ft_strlen(tilde_exp);
	*i += get_tilde_prefix_len(str + *i);
	free(tilde_exp);
	return (len);
}

static size_t	handle_regular_char(const char *str, int *i)
{
	(void)str;
	(*i)++;
	return (1);
}
*/
static int dq_escapable(char c) { return (c=='$' || c=='`' || c=='"' || c=='\\' || c=='\n'); }

size_t	handle_dollar_len(const char *str, int *i, t_shell *shell)
{
	char	*expanded;
	size_t	len;

	(*i)++; // saltar el $
	expanded = expand_value(str, i, shell); // esta función debe avanzar i correctamente
	if (!expanded)
		return ((size_t)-1);
	len = ft_strlen(expanded);
	free(expanded);
	return (len);
}
size_t calculate_expanded_len(const char *str, t_shell *shell)
{
    size_t total = 0;
    int i = 0;
    t_quote q = NO_QUOTE;

    while (str[i])
    {
        if (str[i] == '\'')
        {
            if (q == NO_QUOTE) q = SINGLE_QUOTE;
            else if (q == SINGLE_QUOTE) q = NO_QUOTE;
            total += 1; i++;                  /* la comilla se copia; se quitará en quote_removal */
            continue;
        }
        if (str[i] == '"')
        {
            if (q == NO_QUOTE) q = DOUBLE_QUOTE;
            else if (q == DOUBLE_QUOTE) q = NO_QUOTE;
            total += 1; i++;
            continue;
        }

        if (str[i] == '\\')
        {
            if (q == SINGLE_QUOTE) { total += 1; i++; continue; }   /* literal en '...' */

            if (q == DOUBLE_QUOTE)
            {
                if (str[i+1] && dq_escapable(str[i+1]))
                {
                    if (str[i+1] == '\n') { i += 2; continue; }     /* elimina \+nl */
                    total += 1; i += 2;                             /* copia 1 char, quita '\' */
                    continue;
                }
                total += 1; i++;                                    /* '\' literal */
                continue;
            }

            /* NO_QUOTE */
            if (str[i+1] == '\n') { i += 2; continue; }             /* continuación de línea */
            if (str[i+1]) { total += 1; i += 2; }                   /* quita '\' y copia 1 char */
            else { total += 1; i++; }                               /* '\' final: cópiala */
            continue;
        }

        if (q != SINGLE_QUOTE && str[i] == '$')
        {
            size_t add = handle_dollar_len(str, &i, shell);         /* avanza i internamente */
            if (add == (size_t)-1) return (size_t)-1;
            total += add;
            continue;
        }

        /* Tilde (~) al inicio de palabra o tras '=' */
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
