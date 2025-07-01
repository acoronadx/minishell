/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 16:25:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

static char	*expand_special_vars(const char *str, int *i, t_shell *shell)
{
	char	*res;

	res = NULL;
	if (str[*i] == '?')
	{
		res = ft_itoa(shell->exit_status);
		(*i)++;
	}
	else if (str[*i] == '0')
	{
		res = get_program_name_str(shell);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		res = ft_itoa(getpid());
		(*i)++;
	}
	return (res);
}

static char	*expand_named_var(const char *str, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	start = *i;
	while (str[*i] == '_' || ft_isalnum(str[*i]))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);
	value = find_var(shell->env, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_value(const char *str, int *i, t_shell *shell)
{
	char	*res;

	res = expand_special_vars(str, i, shell);
	if (res)
		return (res);
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (str[*i] == '_' || ft_isalpha(str[*i]))
		return (expand_named_var(str, i, shell));
	return (ft_strdup("$"));
}

int	handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell)
{
	char	*expanded;
	int		len;

	(*i)++; // saltar el $
	expanded = expand_value(str, i, shell); // debe avanzar i igual que en cálculo de len
	if (!expanded)
		return (-1);
	len = ft_strlen(expanded);
	ft_memcpy(res + j, expanded, len);
	free(expanded);
	return (len);
}
