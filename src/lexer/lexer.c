/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 14:40:49 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

/*
** Reglas principales que seguimos (resumen):
** - Comentarios: un '#' que comienza una palabra (inicio de línea, tras espacio
**   no comillado o tras un operador) e *imprimible* y *no comillado* inicia un
**   comentario hasta fin de línea. (Bash: en modo interactivo depende de
**   shopt interactive_comments; aquí lo activamos siempre en no-interactivo).
**   Ref: Bash manual, "Comments". :contentReference[oaicite:0]{index=0}
**
** - Comillas:
**   * '...': literal, sin escapes, hasta la próxima ' (no se puede anidar). :contentReference[oaicite:1]{index=1}
**   * "...": backslash sólo escapa: $, `, ", \ y newline para continuación de
**            línea. (El resto de backslashes se preservan). :contentReference[oaicite:2]{index=2}
**   * fuera de comillas: '\' escapa el siguiente carácter; '\<newline>'
**     elimina ambos (continuación de línea POSIX). :contentReference[oaicite:3]{index=3}
**
** - Operadores reconocidos: ||, &&, |, ;, &, <, >, >>, <<, <<<, >|, <&, >&,
**   2>, 2>>, &>, &>>, paréntesis y llaves. (Algunos son Bash-specific, p.ej. &>, <<<). :contentReference[oaicite:4]{index=4}
**
** Nota: la *expansión* ($VAR, ~, etc.) no es parte del lexer; aquí sólo
** tokenizamos y marcamos si el token estaba comillado.
*/

static t_token *free_null_token_list(t_token **lst)
{
    free_token_list(*lst);
    *lst = NULL;
    return (NULL);
}

/* Detecta prefijos de fd redundantes: "1>", "0<", "2>&1"...;
 * aquí solo los saltamos para simplificar equivalencias de bash.
 * (No altera el AST, sólo evita tokens numéricos sueltos). */
static int  skip_default_fd_prefix(const char *s)
{
    if (s[0] == '1' && (s[1] == '>' || (s[1] == '<' && s[2] == '<')))
        return 1; /* 1>, 1<< */
    if (s[0] == '0' && (s[1] == '<' || (s[1] == '<' && s[2] == '<')))
        return 1; /* 0<, 0<< */
    if (s[0] == '2' && s[1] == '>' && s[2] == '&' && s[3] == '1')
        return 1; /* 2>&1 */
    return 0;
}

/* Sufijos fd redundantes tras un operador (>1, >>1, <&0, >&1...) */
static int  skip_default_fd_suffix(const char *s)
{
    if (s[0] == '>' && s[1] == '1' && s[2] != '>')
        return 1; /* >1 */
    if (s[0] == '>' && s[1] == '>' && s[2] == '1')
        return 1; /* >>1 */
    if (s[0] == '<' && s[1] == '&' && s[2] == '0')
        return 1; /* <&0 */
    if (s[0] == '>' && s[1] == '&' && s[2] == '1')
        return 1; /* >&1 */
    if (s[0] == '<' && s[1] == '<' && s[2] == '<' && s[3] == '1')
        return 1; /* <<<1 */
    return 0;
}

t_token *lexer(const char *raw_line)
{
    t_token *lst = NULL;
    int     i = 0, next_i = 0;

    if (!raw_line)
        return NULL;

    /* Hacemos una copia mutable para poder cortar comentarios de forma segura */
    char *line = ft_strdup(raw_line);
    if (!line)
        return NULL;

    /* Comentarios estilo bash: si # comienza palabra descomillada -> corta línea. */
    strip_comment_if_applicable(line); /* :contentReference[oaicite:5]{index=5} */

    while (line[i])
    {
        /* Saltar espacios fuera de comillas */
        while (line[i] && ft_isspace(line[i]))
            i++;
        if (!line[i])
            break;

        /* Normaliza casos 1>, 0<, 2>&1 (los fd explícitos por defecto) */
        i += skip_default_fd_prefix(line + i);

        if (is_operator(line + i, NULL, NULL))
        {
            next_i = get_operator(line, i, &lst);
            if (next_i < 0)
                return free_null_token_list(&lst);
            /* Saltar sufijos redundantes tras el operador */
            next_i += skip_default_fd_suffix(line + next_i);
        }
        else
        {
            next_i = get_word(line, i, &lst);
            if (next_i < 0)
                return free_null_token_list(&lst);
        }
        i = next_i;
    }

    free(line);
    return lst;
}
