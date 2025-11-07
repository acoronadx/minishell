/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 16:18:25 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_variables(t_shell *shell)
{
	t_token	*t;
	char	*expanded;

	t = shell->tokens;
	while (t)
	{
		expanded = expand_token(t->value, shell);
		if (!expanded)
			return (-1);
		free(t->value);
		t->value = expanded;
		t = t->next;
	}
	perform_word_splitting(shell);
	remove_quotes(shell->tokens);
	return (0);
}
