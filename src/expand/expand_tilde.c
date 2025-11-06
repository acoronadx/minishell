/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:46:11 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:33:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "expand.h"
#include "minishell.h"

char	*expand_tilde_internal(const char *str, t_shell *shell)
{
	char	*home;
	char	*out;

	if (!str || *str != '~')
		return (ft_strdup(str));
	if (ft_strlen(str) == 1 || str[1] == '/')
	{
		home = find_var(shell->env, "HOME");
		if (!home || *home == '\0')
			return (ft_strdup(str));
		if (ft_strlen(str) == 1)
			return (ft_strdup(home));
		out = ft_strjoin(home, str + 1); /* "~" + "/..." */
		return (out);
	}
	/* ~user -> no implementado: copia tal cual */
	return (ft_strdup(str));
}

size_t	get_tilde_prefix_len(const char *str)
{
	if (!str || *str != '~')
		return (0);
	if (ft_strlen(str) == 1 || str[1] == '/')
		return (1 + (str[1] == '/'));
	return (1);
}
