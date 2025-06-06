/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:56:27 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:56:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_line(t_shell *shell, char *line)
{
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '\0')
			break;
		if (ft_isquote(*line))
		{
			int expand;
			char *token = ft_strinquotes(line, &expand);
			ft_add_token(shell, token, T_WORD, 1, expand);
			line += ft_strlen(token) + 2;
			free(token);
		}
		else if (ft_issymbol(*line))
		{
			if (ft_ispipe(*line))
				ft_add_token(shell, "|", T_PIPE, 0, 0);
			else if (ft_isredirection(*line))
				line += ft_add_redirection(shell, line);
			else if (*line == ';')
				ft_add_token(shell, ";", T_SEMICOLON, 0, 0);
			else if (*line == '&')
				ft_add_token(shell, "&", T_BACKGROUND, 0, 0);
			// aquí más símbolos que quieras añadir
		}
		else if (ft_isenvvar(*line))
			ft_add_envvar(shell, line);
		else
			line += ft_add_word(shell, line);
		line++;
	}
}


