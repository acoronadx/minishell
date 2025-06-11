/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:12:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:26:35 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Expande las variables presentes en el string str usando el entorno shell.
** Devuelve una nueva cadena con las variables expandidas.
*/
char	*expand_token(const char *str, t_shell *shell)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = ft_calloc(ft_strlen(str) * 2 + 1, 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			j = handle_dollar(str, &i, res, j, shell);
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

/*
** Expande todas las variables de los argumentos de cada comando de la lista.
** Solo expande si el argumento no está entre comillas simples.
*/
void	expand_variables(t_command *cmd_list, t_shell *shell)
{
	t_command	*cmd;
	t_token		*arg;
	char		*expanded;

	cmd = cmd_list;
	while (cmd)
	{
		arg = cmd->args;
		while (arg)
		{
			if (arg->quoted != SINGLE_QUOTE)
			{
				expanded = expand_token(arg->value, shell);
				free(arg->value);
				arg->value = expanded;
			}
			arg = arg->next;
		}
		cmd = cmd->next;
	}
}
