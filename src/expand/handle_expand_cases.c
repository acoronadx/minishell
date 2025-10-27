/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expand_cases.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:08:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:33:35 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

size_t handle_exit_status_len(t_shell *shell)
{
    char   *s = ft_itoa(shell->exit_status);
    size_t  n;

    if (!s) return (size_t)-1;
    n = ft_strlen(s);
    free(s);
    return n;
}

size_t handle_pid_len(void)
{
    char   *s = ft_itoa(getpid());
    size_t  n;

    if (!s) return (size_t)-1;
    n = ft_strlen(s);
    free(s);
    return n;
}
