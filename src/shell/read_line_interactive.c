/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line_interactive.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 16:38:56 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/05 09:07:23 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helpers de cierre de comillas/backslash (mis versiones) */
static int	ft_quotes_closed(const char *line)
{
	int	i;
	int	qt;

	i = 0;
	qt = 0;
	while (line && line[i])
	{
		if (!qt && (line[i] == '\'' || line[i] == '"'))
			qt = line[i];
		else if (qt == '\'' && line[i] == '\'')
			qt = 0;
		else if (qt == '"' && line[i] == '\\' && (line[i + 1] == '"' || line[i
				+ 1] == '\\' || line[i + 1] == '`'))
			i++;
		else if (qt == '"' && line[i] == '"')
			qt = 0;
		else if (!qt && line[i] == '\\' && line[i + 1])
			i++;
		i++;
	}
	return (qt == 0);
}

static int	ft_backslash_closed(const char *s)
{
	int	i;
	int	qt;
	int	n;

	if (!s)
		return (1);
	qt = 0;
	i = 0;
	while (s[i])
	{
		if (!qt && (s[i] == '\'' || s[i] == '"'))
			qt = s[i];
		else if (qt && s[i] == qt)
			qt = 0;
		i++;
	}
	if (qt == '\'')
		return (1);
	n = (int)ft_strlen(s) - 1;
	i = n;
	while (i >= 0 && s[i] == '\\')
		i--;
	return (((n - i) & 1) == 0);
}

char	*read_line_interactive(t_shell *shell)
{
	setup_prompt_signals();
	g_signal = 0;
	shell->line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
	if (!shell->line)
		return (NULL);
	while (!ft_quotes_closed(shell->line) || !ft_backslash_closed(shell->line))
	{
		if (append_next_line(&shell->line, "> ") < 0)
		{
			free(shell->line);
			shell->line = NULL;
			return (NULL);
		}
	}
	return (shell->line);
}
