/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:32:52 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 15:58:45 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** free_env_list
** Libera toda la memoria de la lista enlazada de variables de entorno (t_env).
** - Recorre la lista nodo por nodo, liberando 'key', 'value' y el propio nodo.
** - Al finalizar, todos los elementos de la lista quedan correctamente liberados.
**
** Parámetros:
**   env: puntero al inicio de la lista enlazada de entorno (t_env*)
**
** Uso:
**   Llama a esta función al salir de la shell o cuando debas liberar todo el entorno.
*/
void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
