/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:22:50 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 12:23:57 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

// ----------- ECHO -----------
int	run_echo(char **argv)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 3) == 0)
	{
		nl = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
