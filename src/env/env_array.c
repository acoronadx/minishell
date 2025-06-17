/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:32:01 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:15:07 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** env_count_exported
** Cuenta cuántas variables de entorno están marcadas como exportadas.
** - Recorre la lista enlazada de entorno (t_env).
** - Suma uno por cada nodo cuyo campo 'exported' es distinto de cero.
** - Devuelve el número total de variables exportadas.
**
** Parámetros:
**   env: inicio de la lista enlazada de entorno (t_env*)
*/
int	env_count_exported(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}

/*
** env_list_to_envp
** Crea un array de strings "KEY=VALUE" solo con las variables exportadas.
** - Cuenta primero cuántas variables exportadas hay.
** - Reserva memoria para el array (incluyendo el NULL final).
** - Copia cada variable exportada al array en formato "clave=valor".
** - Usa ft_strjoin para unir clave y '=' y ft_strjoin_free para añadir el valor.
** - Devuelve el array terminado, listo para pasar a execve.
**
** Parámetros:
**   env: inicio de la lista enlazada de entorno (t_env*)
**
** Devuelve:
**   Un array de strings, cada una en formato "KEY=VALUE", solo variables exportadas.
**   El array termina en NULL. Devuelve NULL si falla la reserva de memoria.
*/
char	**env_list_to_envp(t_env *env)
{
	int		len;
	int		i;
	char	**envp;
	char	*tmp;

	len = env_count_exported(env);
	envp = (char **)ft_calloc(len + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported)
		{
			tmp = ft_strjoin(env->key, "=");
			envp[i] = ft_strjoin_free_s1(tmp, env->value);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
