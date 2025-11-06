/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:28:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 16:53:03 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "minishell.h"

// ----------- EXIT -----------

/* Comprueba si s es un entero con signo en rango int64 */
static int	parse_ll(const char *s, long long *out)
{
	char		*end;
	long long	v;

	end = NULL;
	errno = 0;
	if (!s || !*s)
		return (0);
	v = strtoll(s, &end, 10);
	if (errno == ERANGE)
		return (0);
	if (*end != '\0')
		return (0);
	*out = v;
	return (1);
}

int	run_exit(char **argv, t_shell *shell)
{
	int				argc;
	int				code;
	long long		val;
	unsigned char	ec;

	/* Bash imprime "exit" en interactivo; tu tester no lo exige. */
	argc = 0;
	while (argv && argv[argc])
		argc++;
	if (argc == 1)
	{
		code = shell ? (shell->exit_status & 0xFF) : 0;
		exit(code);
	}
	val = 0;
	if (!parse_ll(argv[1], &val))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(2); /* POSIX/Bash: sale con 2 si no numérico */
	}
	if (argc > 2)
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		/* NO salir; status 1 y continuar */
		return (1);
	}
	/* Bash usa el byte menos significativo (módulo 256) */
	ec = (unsigned char)val;
	exit((int)ec);
	return ((int)ec); /* inalcanzable, pero calma analizadores */
}
