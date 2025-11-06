/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_modes_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:56:21 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 14:36:17 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ¿Es fin de opciones? (solo "--") */
static int	is_end_of_opts(const char *s)
{
	if (!s)
		return (0);
	if (s[0] == '-' && s[1] == '-' && s[2] == '\0')
		return (1);
	return (0);
}

/* ¿Tiene forma de opción corta? ("-x") */
static int	is_short_opt(const char *s)
{
	if (!s)
		return (0);
	if (s[0] == '-' && s[1] != '\0')
		return (1);
	return (0);
}

/* -c <string>: toma el argumento o error 2 */
static int	take_c_arg_simple(int argc, char **argv, int *i, t_opts *o)
{
	*i += 1;
	if (*i >= argc)
	{
		ft_dprintf(2, "minishell: -c requires an argument\n");
		return (2);
	}
	o->cstr = argv[*i];
	return (0);
}

/* Despacha "-i", "-s", "-c": 0 ok, 1 stop (no-opción), 2 error */
static int	dispatch_opt(int argc, char **argv, int *i, t_opts *o)
{
	if (ft_strcmp(argv[*i], "-i") == 0)
	{
		o->flag_i = 1;
		return (0);
	}
	if (ft_strcmp(argv[*i], "-s") == 0)
	{
		o->flag_s = 1;
		return (0);
	}
	if (ft_strcmp(argv[*i], "-c") == 0)
		return (take_c_arg_simple(argc, argv, i, o));
	return (1);
}

/* Acepta SOLO: -i  -s  -c <string>  y '--' */
int	parse_args_simple(int argc, char **argv, t_opts *o)
{
	int	i;
	int	ret;

	o->flag_i = 0;
	o->flag_s = 0;
	o->cstr = NULL;
	i = 1;
	while (i < argc && is_short_opt(argv[i]))
	{
		if (is_end_of_opts(argv[i]))
		{
			i++;
			break ;
		}
		ret = dispatch_opt(argc, argv, &i, o);
		if (ret == 2)
			return (2);
		if (ret == 1)
			break ;
		i++;
	}
	o->first_nonopt = i;
	return (0);
}
