/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:26:37 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/06 12:25:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* nombre válido POSIX-like: [_A-Za-z][_A-Za-z0-9]* */
static int	is_valid_name(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static t_env	*env_find(t_env *e, const char *key)
{
	while (e)
	{
		if (e->key && key && ft_strcmp(e->key, key) == 0)
			return (e);
		e = e->next;
	}
	return (NULL);
}

/* crea si no existe (valor "" si mark_only), marca exportada; si val y !mark_only, actualiza */
static void	env_set_exported(t_shell *sh, const char *key, const char *val, int mark_only)
{
	t_env	*n;
	char	*newv;

	n = env_find(sh->env, key);
	if (!n)
	{
		newv = (mark_only || !val) ? "" : (char *)val;
		n = env_create((char *)key, newv, 1);
		if (!n)
			return ;
		env_add_back(&sh->env, n);
		return ;
	}
	n->exported = 1;
	if (!mark_only && val)
	{
		if (n->value)
			free(n->value);
		n->value = ft_strdup(val);
	}
}

static int	export_mark_only(t_shell *sh, const char *name)
{
	if (!is_valid_name(name))
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", name);
		return (1);
	}
	env_set_exported(sh, name, NULL, 1);
	return (0);
}

/* procesa NAME=VALUE sin modificar argv */
static int	export_with_value(t_shell *sh, const char *arg)
{
	const char	*eq;
	char		*name;
	char		*value;
	int			name_len;
	int			rc;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (export_mark_only(sh, arg));
	name_len = (int)(eq - arg);
	name = ft_substr(arg, 0, name_len);
	if (!name)
		return (1);
	if (!is_valid_name(name))
	{
		ft_dprintf(2, "minishell: export: `%s=': not a valid identifier\n", name);
		free(name);
		return (1);
	}
	value = ft_strdup(eq + 1);
	if (!value)
		return (free(name), 1);
	env_set_exported(sh, name, value, 0);
	rc = 0;
	free(name);
	free(value);
	return (rc);
}

/* builtin: export [NAME[=VALUE]]... ; sin args: (pendiente) imprimir declare -x */
int	run_export(char **argv, t_shell *shell)
{
	int	i;
	int	status;

	if (!argv || !shell)
		return (1);
	if (!argv[1])
		return (0);
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (export_with_value(shell, argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}
