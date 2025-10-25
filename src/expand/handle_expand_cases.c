/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expand_cases.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:08:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:42:47 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
size_t handle_dollar_len(const char *str, int *i, t_shell *shell)
{
    // str[*i] ya es el caracter justo después de '$'
    if (str[*i] == '?')
        return (handle_exit_status_len(shell));
    else if (str[*i] == '0')
    {
        if (shell->program_name)
            return (ft_strlen(shell->program_name));
        else
            return (ft_strlen("minishell"));
    }
    else if (str[*i] == '$')
        return (handle_pid_len());
    else if (ft_isdigit(str[*i]))
        return (0);
    else
        return (1);
}
*/

size_t handle_exit_status_len(t_shell *shell)
{
    char    *itoa_exit;
    size_t  len;

    itoa_exit = ft_itoa(shell->exit_status);
    if (!itoa_exit) return ((size_t)-1);
    len = ft_strlen(itoa_exit);
    free(itoa_exit);
    return (len);
}

size_t handle_pid_len(void)
{
    char    *itoa_pid;
    size_t  len;

    itoa_pid = ft_itoa(getpid());
    if (!itoa_pid) return ((size_t)-1);
    len = ft_strlen(itoa_pid);
    free(itoa_pid);
    return (len);
}
