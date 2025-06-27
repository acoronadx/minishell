/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 15:30:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static t_token *free_null_token_list(t_token **lst)
{
    free_token_list(*lst);
    *lst = NULL;
    return (NULL);
}

t_token	*lexer(const char *line)
{
	int		i;
	int		next_i;
	t_token	*lst;

	i = 0;
	lst = NULL;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		next_i = i;
		if (line[i] == '\'' || line[i] == '"')
			next_i = get_quoted(line, i, &lst);
		else if (is_operator(line + i, NULL, NULL))
			next_i = get_operator(line, i, &lst);
		else
			next_i = get_word(line, i, &lst);
		if (next_i == -1)
			return (free_null_token_list(&lst));
		i = next_i;
	}
	return (lst);
}
