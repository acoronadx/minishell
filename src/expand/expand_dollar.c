/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 18:13:47 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
static char	*remove_empty_quotes_post_expand(const char *src)
{
	char	*clean;
	int		len;

	len = ft_strlen(src);
	if (len < 2)
		return (ft_strdup(src));
	if ((src[len - 2] == '"' && src[len - 1] == '"')
		|| (src[len - 2] == '\'' && src[len - 1] == '\''))
	{
		clean = ft_substr(src, 0, len - 2);
		if (!clean)
			return (NULL);
		return (clean);
	}
	return (ft_strdup(src));
}
*/
/*
static char	*expand_special_vars(const char *str, int *i, t_shell *shell)
{
	char	*res;

	res = NULL;
	if (str[*i] == '?')
	{
		res = ft_itoa(shell->exit_status);
		(*i)++;
	}
	else if (str[*i] == '0')
	{
		res = get_program_name_str(shell);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		res = ft_itoa(getpid());
		(*i)++;
	}
	return (res);
}

static char	*expand_named_var(const char *str, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	start = *i;
	while (str[*i] == '_' || ft_isalnum(str[*i]))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);
	value = find_var(shell->env, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
*/

char *expand_value(const char *s, int *i, t_shell *shell)
{
    /* s[*i] apunta al carácter JUSTO DESPUÉS de '$' */
    if (s[*i] == '?') {                 /* $? -> exit status */
        (*i)++;
        return ft_itoa(shell->exit_status);
    }
    if (s[*i] == '$') {                 /* $$ -> pid del shell */
        (*i)++;
        return ft_itoa(getpid());
    }
    if (s[*i] == '0') {                 /* $0 -> nombre del programa */
        (*i)++;
        return get_program_name_str(shell);
    }

    /* Parámetros posicionales $1..$9 (si no los implementas): vacío */
    if (ft_isdigit((unsigned char)s[*i])) {
        (*i)++;
        return ft_strdup("");
    }

    /* $ seguido de comillas o fin de palabra -> vacío (p.ej. $"") */
    if (!s[*i] || s[*i] == '"' || s[*i] == '\'')
        return ft_strdup("");

    /* $VAR o $_... */
    if (ft_isalpha((unsigned char)s[*i]) || s[*i] == '_') {
        int start = *i;
        while (ft_isalnum((unsigned char)s[*i]) || s[*i] == '_')
            (*i)++;
        char *name = ft_substr(s, start, *i - start);
        if (!name) return NULL;
        char *val = find_var(shell->env, name);
        free(name);
        return ft_strdup(val ? val : "");
    }

    /* Caso por defecto: tratar '$' como literal.
       MUY IMPORTANTE: aquí NO avanzamos *i, para no “comernos” el carácter que sigue
       (p.ej. "$-"). El caller ya consumió el '$'. */
    return ft_strdup("$");
}


int handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell)
{
    char    *expanded;
    int     len;

    (*i)++; // saltar el $
    expanded = expand_value(str, i, shell);
    if (!expanded)
        return (-1);
    len = ft_strlen(expanded);
    ft_memcpy(res + j, expanded, len);
    free(expanded);
    return (len);
}
