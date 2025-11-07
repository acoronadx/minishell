/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:22:50 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 20:36:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_all_n(const char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	if (s[i] == '\0')
		return (0); // solo "-" no cuenta
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1); // "-n", "-nn", "-nnn", ...
}

int	run_echo(char **argv)
{
	int	i;
	int	no_newline;
	int	first_arg_printed;

	i = 1;
	no_newline = 0;
	while (argv[i] && is_all_n(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	first_arg_printed = 0;
	while (argv[i])
	{
		if (first_arg_printed)
			write(1, " ", 1);
		write(1, argv[i], ft_strlen(argv[i]));
		first_arg_printed = 1;
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}
