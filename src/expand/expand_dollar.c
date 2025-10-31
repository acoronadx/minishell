/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/31 21:34:27 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

char    *expand_value(const char *s, int *i, t_shell *shell)
{
    if (!s || !s[*i])
        return (ft_strdup(""));

    if (s[*i] == '?')
    {
        (*i)++;
        return (ft_itoa(shell->exit_status));
    }
    if (s[*i] == '$')
    {
        (*i)++;
        return (ft_itoa(getpid()));
    }
    if (s[*i] == '0')
    {
        (*i)++;
        return (get_program_name_str(shell));
    }
    if (ft_isdigit((unsigned char)s[*i]))
    {
        (*i)++;
        return (ft_strdup(""));
    }
    if (s[*i] == '"' || s[*i] == '\'' || s[*i] == '\0')
        return (ft_strdup(""));

    if (ft_isalpha((unsigned char)s[*i]) || s[*i] == '_')
    {
        int     start;
        char    *name;
        char    *val;

        start = *i;
        while (ft_isalnum((unsigned char)s[*i]) || s[*i] == '_')
            (*i)++;
        name = ft_substr(s, start, *i - start);
        if (!name)
            return (NULL);
        val = find_var(shell->env, name);
        free(name);
        return (ft_strdup(val ? val : ""));
    }
    (*i)++;
    return (ft_strdup("$"));
}
