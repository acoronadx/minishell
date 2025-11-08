/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/08 16:56:05 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *clone_and_strip(const char *raw)
{
    char *dup;

    if (!raw)
        return (NULL);
    dup = ft_strdup(raw);
    if (!dup)
        return (NULL);
    strip_comment_if_applicable(dup);
    return (dup);
}

static int skip_spaces(const char *s, int i)
{
    while (s[i] && ft_isspace((unsigned char)s[i]))
        i++;
    return (i);
}

static int lex_one(const char *line, int i, t_token **lst, int *next_i)
{
    int n;

    if (is_operator(line + i, NULL, NULL))
    {
        n = get_operator(line, i, lst);
        if (n < 0)
            return (-1);
        *next_i = n;
        return (0);
    }
    n = get_word(line, i, lst);
    if (n < 0)
        return (-1);
    *next_i = n;
    return (0);
}

t_token *lexer(const char *raw_line)
{
    t_token *lst;
    char    *line;
    int      i;
    int      next_i;

    lst = NULL;
    line = clone_and_strip(raw_line);
    if (!line)
        return (NULL);
    i = 0;
    while (line[i])
    {
        i = skip_spaces(line, i);
        if (!line[i])
            break ;
        if (lex_one(line, i, &lst, &next_i) < 0)
            return (free(line), free_null_token_list(&lst));
        i = next_i;
    }
    free(line);
    /* 🔑 Marca delimitadores de heredoc y si venían entrecomillados */
    mark_heredoc_delims(lst);
    return (lst);
}
