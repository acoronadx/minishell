/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:28:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 12:31:54 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

// ----------- EXIT -----------

int	run_exit(char **argv, t_shell *shell)
{
	long	code;

	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
		exit(shell->exit_status);
	if (!ft_isnumeric(argv[1]))
	{
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", argv[1]);
		exit(255);
	}
	if (argv[2])
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		return (1);
	}
	code = ft_atol(argv[1]);
	code = code & 255;
	exit((int)code);
}
