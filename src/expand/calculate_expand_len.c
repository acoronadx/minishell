/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_expand_len.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:16:36 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 16:31:03 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

static size_t	handle_tilde_len(const char *str, int *i, t_shell *shell)
{
	char	*tilde_exp;
	size_t	len;

	tilde_exp = expand_tilde_internal(str + *i, shell);
	if (!tilde_exp)
		return ((size_t)-1);
	len = ft_strlen(tilde_exp);
	*i += get_tilde_prefix_len(str + *i);
	free(tilde_exp);
	return (len);
}

static size_t	handle_regular_char(const char *str, int *i)
{
	(void)str;
	(*i)++;
	return (1);
}

size_t	handle_dollar_len(const char *str, int *i, t_shell *shell)
{
	char	*expanded;
	size_t	len;

	(*i)++; // saltar el $
	expanded = expand_value(str, i, shell); // esta función debe avanzar i correctamente
	if (!expanded)
		return ((size_t)-1);
	len = ft_strlen(expanded);
	free(expanded);
	return (len);
}

size_t	calculate_expanded_len(const char *str, t_shell *shell)
{
	size_t	total;
	int		i;
	size_t	len;

	total = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			len = handle_dollar_len(str, &i, shell);
		}
		else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1])))
		{
			len = handle_tilde_len(str, &i, shell);
		}
		else
		{
			len = handle_regular_char(str, &i);
		}
		if (len == (size_t)-1)
			return ((size_t)-1);
		total += len;
	}
	return (total);
}
