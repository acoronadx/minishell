/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 14:42:10 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/27 20:01:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:28:27 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 06:18:34 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

/*
** Crea un nuevo nodo de variable de entorno (t_env).
** - key: nombre de la variable (ej: "USER")
** - value: valor de la variable (ej: "acoronadx")
** - exported: 1 si la variable está exportada, 0 si es local/interna
** Devuelve un puntero al nuevo nodo, o NULL si falla el malloc.
** El campo .next se inicializa a NULL.
*/
t_env	*env_create(char *key, char *value, int exported)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->exported = exported;
	new->next = NULL;
	return (new);
}

/*
** Añade un nodo t_env al final de la lista de entorno.
** - *env: puntero al inicio de la lista (puede ser NULL si está vacía)
** - new: nuevo nodo a añadir
** Si la lista está vacía, pone el nuevo como primer elemento.
** Si no, recorre la lista y lo añade al final.
** No hace nada si alguno de los punteros es NULL.
*/

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!env || !new)
		return ;
	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

/*
** Inicializa la lista enlazada de entorno (t_env) a partir del array envp.
** - envp: array de strings "KEY=VALUE" que viene de main().
** Por cada string, separa la clave y el valor, crea un nodo y lo añade a la lista.
** Marca todas las variables como exportadas (exported = 1).
** Devuelve el inicio de la lista, o NULL si falla algún malloc.
** Ejemplo de uso: shell->env = init_env_list(envp);
*/
static t_env	*create_exported_env_var(char *str, t_env **env)
{
	char	*sep;
	char	*key;
	t_env	*new;

	sep = ft_strchr(str, '=');
	key = ft_substr(str, 0, sep - str);
	if (!key)
	{
		free_env_list(*env);
		return (NULL);
	}
	new = env_create(key, sep + 1, 1);
	free(key);
	if (!new)
	{
		free_env_list(*env);
		return (NULL);
	}
	env_add_back(env, new);
	return (*env);
}

t_env	*init_env_list(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			if (!create_exported_env_var(envp[i], &env))
				return (NULL);
		}
		i++;
	}
	return (env);
}
