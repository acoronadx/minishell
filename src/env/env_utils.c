/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:31:08 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:32:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if ((str[0] >= '0' && str[0] <= '9') || str[0] == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!((str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

// 🔹 get_env_value
// Devuelve el valor de una variable SOLO si está exportada (exported == 1).
// ➤ ÚSALA cuando vayas a ejecutar un comando externo que hereda el entorno.
//
// Ejemplo: al construir el array `envp` para `execve`, expansión de $VAR heredables, etc.
char *get_env_value(t_env *env, const char *key)
{
	t_env	*node = get_env_node(env, key);
	if (node && node->exported)
		return (node->value);
	return (NULL);
}

// 🔹 get_env_value_any
// Devuelve SOLO el valor de una variable del entorno (env->value),
// sin importar si está exportada o no.
// ➤ Si la variable no existe, devuelve NULL.
//
// 📌 ÚSALA cuando solo necesites el valor de una variable,
//     sin modificarla ni tocar otros campos.
// ➤ ÚSALA dentro del propio shell, para lógica interna, comandos como `export`, etc.
//
// 🧠 Es útil para expansión de variables internas del shell
//     o lógica que no tiene que ver con exportación.
// Ejemplo: al actualizar una variable que el usuario ha definido pero no ha exportado todavía.
char *get_env_value_any(t_env *env, const char *key)
{
	t_env	*node = get_env_node(env, key);
	if (node)
		return (node->value);
	return (NULL);
}

// 🔹 get_env_node
// Devuelve el nodo COMPLETO de la variable de entorno (t_env *),
// incluyendo key, value, exported y next.
//
// 📌 ÚSALA cuando NECESITES MODIFICAR la variable, por ejemplo:
//     - actualizar su valor
//     - cambiar su estado de `exported`
//     - borrarla (con unset, etc.)
//     - ver más información de la variable
//
// 🧠 También es útil cuando quieres comprobar si existe una variable
//     y trabajar con toda su estructura.

t_env	*get_env_node(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_env_path(t_env *env_list, const char *cmd)
{
	char	*path_value;
	char	**dirs;
	char	*full;

	// 🔹 Busca el valor de PATH usando la lista enlazada
	path_value = get_env_value(env_list, "PATH");
	if (!path_value)
		return (NULL);
	dirs = ft_split_custom(path_value, ':');
	if (!dirs)
		return (NULL);
	full = search_cmd_in_dirs(dirs, cmd);
	free_split(dirs);
	return (full);
}
