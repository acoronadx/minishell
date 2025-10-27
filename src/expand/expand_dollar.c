/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:33:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

char *expand_value(const char *s, int *i, t_shell *shell)
{
    /* s[*i] es el carácter justo DESPUÉS del '$' */
    if (!s || !s[*i])
        return ft_strdup("");

    /* especiales */
    if (s[*i] == '?') { (*i)++; return ft_itoa(shell->exit_status); } /* $? */
    if (s[*i] == '$') { (*i)++; return ft_itoa(getpid()); }           /* $$ */
    if (s[*i] == '0') { (*i)++; return get_program_name_str(shell); } /* $0 */

    /* posicionales $1..$9 -> vacías si no existen */
    if (ft_isdigit((unsigned char)s[*i])) { (*i)++; return ft_strdup(""); }

    /* "$" seguido de comilla o fin -> "" (coincide con tus tests) */
    if (s[*i] == '"' || s[*i] == '\'' || s[*i] == '\0')
        return ft_strdup("");

    /* nombre de variable */
    if (ft_isalpha((unsigned char)s[*i]) || s[*i] == '_')
    {
        int start = *i;
        while (ft_isalnum((unsigned char)s[*i]) || s[*i] == '_') (*i)++;
        char *name = ft_substr(s, start, *i - start);
        if (!name) return NULL;
        char *val = find_var(shell->env, name);
        free(name);
        return ft_strdup(val ? val : "");
    }

    /* por defecto: trata el '$' como literal */
    (*i)++;
    return ft_strdup("$");
}

int handle_dollar(const char *s, int *i, char *dst, int j, t_shell *shell)
{
    char *expanded;
    int   len;

    (*i)++; /* saltar '$' en el input */
    expanded = expand_value(s, i, shell);
    if (!expanded) return -1;
    len = (int)ft_strlen(expanded);
    ft_memcpy(dst + j, expanded, len);
    free(expanded);
    return len;
}
