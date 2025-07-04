/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/03 19:21:02 by acoronad         ###   ########.fr       */
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

// Helper: detecta si hay un "1>", "1>>", "1<<", "0<", "2>&1", ">&1", "<&0", etc. 
// y devuelve el desplazamiento para que el lexer trate todos los casos equivalentes.

static int	is_default_fd_redir(const char *line)
{
	if (line[0] == '1')
	{
		if (line[1] == '>' && line[2] == '>')
			return (1); // 1>> igual que >>
		if (line[1] == '>')
			return (1); // 1> igual que >
		if (line[1] == '<' && line[2] == '<')
			return (1); // 1<< igual que <<
	}
	if (line[0] == '0')
	{
		if (line[1] == '<' && line[2] == '<')
			return (1); // 0<< igual que <<
		if (line[1] == '<')
			return (1); // 0< igual que <
	}
	if (line[0] == '2')
	{
		if (line[1] == '>' && line[2] == '&' && line[3] == '1')
			return (1); // 2>&1 igual que >&1
	}
	return (0);
}

// Helper: detecta si tras el operador hay un fd por defecto y lo ignora (ej: >1, >>1, <<1, >&1, <&0)
static int	is_default_fd_suffix(const char *line)
{
	if (line[0] == '>' && line[1] == '1' && line[2] != '>')
		return (1); // >1 igual que >
	if (line[0] == '>' && line[1] == '>' && line[2] == '1')
		return (1); // >>1 igual que >>
	if (line[0] == '<' && line[1] == '<' && line[2] == '1')
		return (1); // <<1 igual que <<
	if (line[0] == '>' && line[1] == '&' && line[2] == '1')
		return (1); // >&1 igual que >&
	if (line[0] == '<' && line[1] == '&' && line[2] == '0')
		return (1); // <&0 igual que <&
	if (line[0] == '<' && line[1] == '<' && line[2] == '<' && line[3] == '1')
		return (1); // <<<1 igual que <<<
	return (0);
}

t_token	*lexer(const char *line)
{
	int		i;
	int		next_i;
	t_token	*lst;
	int		skip_fd;

	i = 0;
	lst = NULL;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		next_i = i;
		skip_fd = is_default_fd_redir(line + i);
		if (skip_fd)
			i += skip_fd;
		if (is_operator(line + i, NULL, NULL))
		{
			next_i = get_operator(line, i, &lst);
			if (is_default_fd_suffix(line + next_i))
				next_i += 1;
		}
		else if (line[i] == '\'' || line[i] == '"')
			next_i = get_quoted(line, i, &lst);
		else
			next_i = get_word(line, i, &lst);
		if (next_i == -1)
			return (free_null_token_list(&lst));
		i = next_i;
	}
	return (lst);
}
