/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:31:07 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 15:58:53 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** add_env_var
** Añade una nueva variable al entorno (lista enlazada t_env).
** - Si la variable con la misma clave ya existe, NO la modifica ni añade.
** - Si no existe, crea un nodo con key, value, exported y lo añade al final.
** - Devuelve 1 si la variable se añadió, 0 si ya existía o hubo error de malloc.
**
** Parámetros:
**   env: puntero doble al inicio de la lista de entorno
**   key: nombre de la variable
**   value: valor de la variable (puede ser NULL)
**   exported: 1 si la variable debe ser exportada, 0 si es solo local
*/
int	add_env_var(t_env **env, const char *key, const char *value, int exported)
{
	t_env	*node;

	if (get_env_node(*env, key))
		return (0);
	node = env_create(key, value, exported);
	if (!node)
		return (0);
	env_add_back(env, node);
	return (1);
}

/*
** update_env_var
** Actualiza el valor y el estado de exportación de una variable existente.
** - Si la variable no existe, NO hace nada y devuelve 0.
** - Si existe, libera el valor antiguo (si lo hay), duplica el nuevo valor
**   y actualiza el campo exported.
** - Devuelve 1 si se actualizó correctamente, 0 si no existe.
**
** Parámetros:
**   env: inicio de la lista enlazada de entorno
**   key: nombre de la variable a actualizar
**   value: nuevo valor (puede ser NULL)
**   exported: nuevo estado de exportación (1 o 0)
*/
int	update_env_var(t_env *env, const char *key, const char *value, int exported)
{
	t_env	*node;

	node = get_env_node(env, key);
	if (!node)
		return (0);
	if (node->value)
		free(node->value);
	node->value = value ? ft_strdup(value) : NULL;
	node->exported = exported;
	return (1);
}

/*
** remove_env_var
** Elimina una variable del entorno por su clave.
** - Si la variable existe, la elimina de la lista, libera su memoria y devuelve 1.
** - Si no existe, no hace nada y devuelve 0.
**
** Parámetros:
**   env: puntero doble al inicio de la lista de entorno
**   key: nombre de la variable a eliminar
*/
int	remove_env_var(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	if (!env || !*env)
		return (0);
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
