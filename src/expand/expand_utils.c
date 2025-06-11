/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:34:10 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:26:17 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Busca el valor de una variable en el entorno env.
** Devuelve puntero a value o "" si no existe la variable.
*/
char	*find_var(t_env *env, char *name)
{
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

/*
** Expande $? insertando el exit_status en res.
** Devuelve el nuevo índice j tras copiar el valor.
*/
int	handle_exit_status(char *res, int j, int *i, t_shell *shell)
{
	char	*itoa_exit;
	int		len;

	itoa_exit = ft_itoa(shell->exit_status);
	len = ft_strlen(itoa_exit);
	ft_strcpy(res + j, itoa_exit);
	free(itoa_exit);
	(*i)++;
	return (j + len);
}

/*
** Expande el nombre de variable a partir de str[*i] y la copia en res.
** Devuelve el nuevo índice j tras copiar el valor.
*/
int	handle_varname(const char *str, int *i, char *res, int j, t_shell *shell)
{
	char	var[256];
	int		k;
	char	*value;
	int		len;

	k = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_') && k < 255)
	{
		var[k] = str[*i];
		k++;
		(*i)++;
	}
	var[k] = '\0';
	value = find_var(shell->env, var);
	len = ft_strlen(value);
	ft_strcpy(res + j, value);
	return (j + len);
}

/*
** Gestiona la expansión cuando encuentra un '$' en la cadena.
** Llama a la función correspondiente según el tipo de variable.
*/
int	handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell)
{
	if (str[*i] == '?')
		return (handle_exit_status(res, j, i, shell));
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
		return (handle_varname(str, i, res, j, shell));
	else
	{
		res[j] = '$';
		j++;
		return (j);
	}
}

