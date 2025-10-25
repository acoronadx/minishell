/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:06:08 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 04:29:30 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signal.h"

/*
** heredoc_read:
** Lee un heredoc hasta el delimitador o interrupción por Ctrl+C (SIGINT).
** - Devuelve un fd de solo lectura si todo va bien.
** - Devuelve -1 si hubo interrupción o error.
*/

/* here-doc: lee hasta 'delimiter' (sin expansión aquí; eso se decide al parsear) */

int heredoc_read(char *delimiter)
{
    int   pipefd[2];
    char *line;

    if (!delimiter)
        return (-1);
    if (pipe(pipefd) < 0)
    {
        perror("minishell: pipe");
        return (-1);
    }

    /* Comportamiento típico: en heredoc el shell hijo maneja SIGINT/SIGQUIT por defecto */
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);

    while (1)
    {
        line = readline("> ");
        if (!line)  /* EOF (Ctrl-D) */
            break;
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        /* Sin expansión: se escribe tal cual con salto de línea */
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);          /* Muy importante: cerrar escritura para que el lector vea EOF */
    return (pipefd[0]);        /* Devuelve fd de lectura */
}
