/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 03:14:32 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 16:35:57 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

char	**add_argument(char **argv, int *argc, int *size, const char *value)
{
	char	**new_argv;
	size_t	old_bytes;
	size_t	new_bytes;

	if (*argc >= *size)
	{
		old_bytes = (size_t)(*size) * sizeof(char *);
		*size *= 2;
		new_bytes = (size_t)(*size + 1) * sizeof(char *);
		new_argv = (char **)ft_realloc(argv, old_bytes, new_bytes);
		if (!new_argv)
		{
			ft_free_strtab(argv);
			return (NULL);
		}
		argv = new_argv;
	}
	argv[*argc] = ft_strdup(value);
	if (!argv[*argc])
	{
		ft_free_strtab(argv);
		return (NULL);
	}
	(*argc)++;
	argv[*argc] = NULL;
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
