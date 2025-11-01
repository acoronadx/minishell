/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:26:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 15:13:13 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"

/* ----------- EXPORT ----------- */
static int	valid_export_key(const char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	/* admite '_' como primer char */
	if (!(ft_isalpha((unsigned char)key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	/* valida hasta '=' si existe */
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum((unsigned char)key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	run_export(char **argv, t_shell *shell)
{
	int		i;
	int		rc;      /* acumula si hay identificadores inválidos */
	char	*sep;
	char	*key;
	char	*value;
	t_env	*envv;

	rc = 0;
	i = 1;
	while (argv[i])
	{
		sep = ft_strchr(argv[i], '=');
		if (sep)
		{
			key = ft_substr(argv[i], 0, sep - argv[i]);
			value = ft_strdup(sep + 1);
			if (valid_export_key(key))
			{
				envv = shell->env;
				while (envv)
				{
					if (ft_strcmp(envv->key, key) == 0)
					{
						free(envv->value);
						envv->value = ft_strdup(value);
						envv->exported = 1;
						break ;
					}
					envv = envv->next;
				}
				if (!envv)
					env_add_back(&shell->env, env_create(key, value, 1));
			}
			else
			{
				ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", key);
				rc = 1; /* ojo: 1 (no 2) */
			}
			free(key);
			free(value);
		}
		else
		{
			if (valid_export_key(argv[i]))
			{
				envv = shell->env;
				while (envv)
				{
					if (ft_strcmp(envv->key, argv[i]) == 0)
					{
						envv->exported = 1;
						break ;
					}
					envv = envv->next;
				}
				if (!envv)
					env_add_back(&shell->env, env_create(argv[i], "", 1));
			}
			else
			{
				ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", argv[i]);
				rc = 1;
			}
		}
		i++;
	}
	return rc;
}
