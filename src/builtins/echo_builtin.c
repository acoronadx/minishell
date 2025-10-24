/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:22:50 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/22 21:00:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

// ----------- ECHO -----------
/*
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
*/

static int is_all_n(const char *s) {
    if (!s || s[0] != '-') return 0;
    for (int i = 1; s[i]; i++)
        if (s[i] != 'n') return 0;
    return (s[1] != '\0'); // al menos un 'n'
}

int run_echo(char **argv)
{
    int i = 1, nflag = 0;
    while (argv[i] && is_all_n(argv[i])) { nflag = 1; i++; }
    for (; argv[i]; i++) {
        write(1, argv[i], ft_strlen(argv[i]));
        if (argv[i + 1]) write(1, " ", 1);
    }
    if (!nflag) write(1, "\n", 1);
    return 0;
}
