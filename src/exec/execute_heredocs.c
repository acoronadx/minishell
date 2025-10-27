/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredocs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:27:07 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 00:51:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int heredoc_prepare(const char *delimiter)
{
    int     pipefd[2];
    char   *line;
    size_t  del_len;

    del_len = ft_strlen(delimiter);
    if (pipe(pipefd) == -1)
        return (-1);

    /* Mientras se lee heredoc: hijo “modalidad lectura” */
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT,  SIG_DFL);

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strncmp(line, delimiter, del_len) == 0 && line[del_len] == '\0')
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);

    /* Restaurar por si otros caminos esperan valores DFL */
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT,  SIG_DFL);
    return (pipefd[0]);
}
