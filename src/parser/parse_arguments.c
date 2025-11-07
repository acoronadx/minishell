/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:14:32 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 07:25:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Duplica la capacidad cuando es necesario. Mantiene terminador NULL extra. */
static char **ensure_capacity(char **argv, int *size, int argc)
{
	char	**new_argv;
	size_t	old_bytes;
	size_t	new_bytes;

	(void)argc;
	old_bytes = (size_t)(*size) * sizeof(char *);
	*size *= 2;
	new_bytes = (size_t)(*size + 1) * sizeof(char *);
	new_argv = (char **)ft_realloc(argv, old_bytes, new_bytes);
	return (new_argv);
}

/* Inserta value al final y mantiene argv[*argc] == NULL */
static int push_arg(char ***pargv, int *argc, const char *value)
{
	char **argv;

	argv = *pargv;
	argv[*argc] = ft_strdup(value);
	if (!argv[*argc])
		return (0);
	(*argc)++;
	argv[*argc] = NULL;
	*pargv = argv;
	return (1);
}

static char	**add_argument(char **argv, int *argc, int *size, const char *value)
{
	char **tmp;

	if (*argc >= *size)
	{
		tmp = ensure_capacity(argv, size, *argc);
		if (!tmp)
		{
			ft_free_strtab(argv);
			return (NULL);
		}
		argv = tmp;
	}
	if (!push_arg(&argv, argc, value))
	{
		ft_free_strtab(argv);
		return (NULL);
	}
	return (argv);
}

char	**parse_arguments(t_token **cur)
{
	int		size;
	int		argc;
	char	**argv;

	size = 4;
	argc = 0;
	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	argv[0] = NULL;
	while (*cur && (*cur)->type == T_WORD)
	{
		argv = add_argument(argv, &argc, &size, (*cur)->value);
		if (!argv)
			return (NULL);
		next_token(cur);
	}
	if (argc == 0)
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}
