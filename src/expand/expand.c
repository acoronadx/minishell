/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/31 21:34:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

static int  copy_expanded_dollar(const char *str, int *i,
                                 t_shell *shell, char *dst, int *j)
{
    char    *expanded;
    int     ii;
    size_t  n;

    ii = *i + 1;
    expanded = expand_value(str, &ii, shell);
    if (!expanded)
        return (-1);
    n = ft_strlen(expanded);
    ft_memcpy(dst + *j, expanded, n);
    *j += (int)n;
    free(expanded);
    *i = ii;
    return (0);
}

char    *expand_token(const char *str, t_shell *shell)
{
    char    *res;
    size_t  need;
    int     i;
    int     j;
    t_quote q;

    if (!str)
        return (NULL);
    need = calculate_expanded_len(str, shell);
    if (need == (size_t)-1)
        return (NULL);
    res = (char *)malloc(need + 1);
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    q = NO_QUOTE;
    while (str[i])
    {
        if (str[i] == '\'')
        {
            if (q == NO_QUOTE) q = SINGLE_QUOTE;
            else if (q == SINGLE_QUOTE) q = NO_QUOTE;
            res[j++] = str[i++];
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
                res[j++] = '\\';
                i++;
                continue;
            }
            if (q == DOUBLE_QUOTE)
            {
                if (str[i + 1] == '\n') { i += 2; continue; }
                if (str[i + 1])
                {
                    res[j++] = '\\';
                    res[j++] = str[i + 1];
                    i += 2;
                    continue;
                }
                res[j++] = '\\';
                i++;
                continue;
            }
            if (str[i + 1] == '\n') { i += 2; continue; }
            if (str[i + 1] == '$') { i++; res[j++] = '$'; i++; continue; }
            res[j++] = '\\';
            i++;
            continue;
        }
        if (q != SINGLE_QUOTE && str[i] == '$')
        {
            if (copy_expanded_dollar(str, &i, shell, res, &j) < 0)
            {
                free(res);
                return (NULL);
            }
            continue;
        }
        if (str[i] == '~'
            && (i == 0 || ft_isspace((unsigned char)str[i - 1]) || str[i - 1] == '='))
        {
            char *t = expand_tilde_internal(str + i, shell);
            if (!t) { free(res); return (NULL); }
            ft_strcpy(res + j, t);
            j += (int)ft_strlen(t);
            i += (int)get_tilde_prefix_len(str + i);
            free(t);
            continue;
        }
        res[j++] = str[i++];
    }
    res[j] = '\0';
    return (res);
}

int     expand_variables(t_shell *shell)
{
    t_token *t;
    char    *expanded;

    t = shell->tokens;
    while (t)
    {
        expanded = expand_token(t->value, shell);
        if (!expanded)
            return (-1);
        free(t->value);
        t->value = expanded;
        t = t->next;
    }
    perform_word_splitting(shell);
    remove_quotes(shell->tokens);
    return (0);
}
