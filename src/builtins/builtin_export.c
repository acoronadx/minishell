/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:38:53 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:39:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int print_export(t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			if (env->value)
				printf("declare -x %s=\"%s\"\n", env->key, env->value);
			else
				printf("declare -x %s\n", env->key);
		}
		env = env->next;
	}
	return (0);
}

static int	parse_key_value(char *arg, char **key, char **value)
{
	char	*sep = ft_strchr(arg, '=');

	if (sep)
	{
		*key = ft_substr(arg, 0, sep - arg);
		*value = ft_strdup(sep + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (sep && !*value))
	{
		if (*key)
			free(*key);
		if (*value)
			free(*value);
		return (0);
	}	
	return (1);
}

static void	update_existing_env(t_env *env, char *value)
{
	if (value)
	{
		free(env->value);
		env->value = value;
	}
	env->exported = 1;
}

static void	create_new_env(t_shell *shell, char *key, char *value)
{
	t_env *new = env_create(key, value, 1);
	if (!new)
		ft_print_exit_init_error("malloc", shell);//esto no vale aquí porque nos hace un cleanup
	env_add_back(&shell->env_list, new);
}

int	add_or_update_env(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	t_env	*existing;

	if (!parse_key_value(arg, &key, &value))
		return (1);
	if (!is_valid_identifier(key))
	{
		printf("minishell: export: `%s`: not a valid identifier\n", arg);
		free(key);
		free(value);
		return (1);
	}
	existing = get_env_node(shell->env_list, key);
	if (existing)
		update_existing_env(existing, value);
	else
		create_new_env(shell, key, value);
	free(key);
	return (0);
}

int	builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	status;

	if (!args[1])
		return (print_export(shell->env_list));
	i = 1;
	status = 0;
	while (args[i])
	{
		if (add_or_update_env(shell, args[i]))
			status = 1;
		i++;
	}
	return (status);
}
