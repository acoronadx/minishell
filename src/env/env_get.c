/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:30:30 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 15:58:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** get_env_node
** Busca un nodo en la lista enlazada de entorno que tenga la clave 'key'.
** - Recorre la lista 'env' comparando cada key con la buscada.
** - Devuelve un puntero al nodo si lo encuentra, o NULL si no existe.
**
** Parámetros:
**   env: inicio de la lista enlazada de variables de entorno (t_env*)
**   key: clave a buscar (ej: "PATH")
*/
t_env	*get_env_node(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
** get_env_value_any
** Devuelve el valor de una variable de entorno por su clave, esté exportada o no.
** - Busca el nodo con la clave 'key' usando get_env_node.
** - Devuelve el campo value del nodo si existe, o NULL si no está en la lista.
**
** Parámetros:
**   env: inicio de la lista enlazada de variables de entorno (t_env*)
**   key: clave a buscar (ej: "PATH")
*/
char	*get_env_value_any(t_env *env, const char *key)
{
	t_env	*node;

	node = get_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

/*
** get_env_value
** Devuelve el valor de una variable de entorno solo si está exportada.
** - Busca el nodo con la clave 'key' usando get_env_node.
** - Solo devuelve el value si exported == 1, o NULL en caso contrario.
**
** Parámetros:
**   env: inicio de la lista enlazada de variables de entorno (t_env*)
**   key: clave a buscar (ej: "PATH")
*/
char	*get_env_value(t_env *env, const char *key)
{
	t_env	*node;

	node = get_env_node(env, key);
	if (node && node->exported)
		return (node->value);
	return (NULL);
}
