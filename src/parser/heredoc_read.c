/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** heredoc_read:
** Lee un heredoc hasta el delimitador o Ctrl+C. Devuelve fd de lectura,
** o -1 en error/interrupción.
*/

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
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    return (pipefd[0]);
}
