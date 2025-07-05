/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/05 16:11:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

static int	skip_empty_quotes(const char *str, int i)
{
	if ((str[i] == '"' && str[i + 1] == '"')
		|| (str[i] == '\'' && str[i + 1] == '\''))
		return (i + 2);
	return (i);
}

char	*expand_token(const char *str, t_shell *shell)
{
	int		i;
	int		j;
	char	*res;
	size_t	required_len;
	int		added;

	i = 0;
	j = 0;
	required_len = calculate_expanded_len(str, shell);
	if (required_len == (size_t)-1)
		return (NULL);
	res = ft_calloc(required_len + 1, 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '$')
		{
			res[j] = '$';
			i += 2;
			j++;
		}
		else if (str[i] == '$')
		{
			added = handle_dollar(str, &i, res, j, shell);
			if (added == -1)
			{
				free(res);
				return (NULL);
			}
			i = skip_empty_quotes(str, i);
			j += added;
		}
		else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1])))
		{
			char *tilde_expanded = expand_tilde_internal(str + i, shell);
			if (!tilde_expanded)
			{
				free(res);
				return (NULL);
			}
			ft_strcpy(res + j, tilde_expanded);
			j += ft_strlen(tilde_expanded);
			i += get_tilde_prefix_len(str + i);
			free(tilde_expanded);
		}
		else
		{
			res[j] = str[i];
			j++;
			i++;
		}
	}
	res[j] = '\0';
	return (res);
}

int	expand_variables(t_shell *shell)
{
	t_token	*current_token;
	char	*expanded_value;

	if (!shell->tokens)
		return (0);
	current_token = shell->tokens;
	while (current_token)
	{
		if (current_token->quoted != SINGLE_QUOTE)
		{
			expanded_value = expand_token(current_token->value, shell);
			if (!expanded_value)
			{
				free_token_list(shell->tokens);
				shell->tokens = NULL;
				return (-1);
			}
			free(current_token->value);
			current_token->value = expanded_value;
		}
		current_token = current_token->next;
	}
	return (0);
}
