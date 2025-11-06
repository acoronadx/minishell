/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 15:35:59 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "expand.h"
#include "minishell.h"

/* ========= helpers existentes (sin cambios de interfaz) ========= */

static char	*dup_program_name(t_shell *shell)
{
	if (!shell || !shell->program_name)
		return (ft_strdup("minishell"));
	return (ft_strdup(shell->program_name));
}

static char	*expand_specials(const char *s, int *i, t_shell *shell)
{
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	if (s[*i] == '$')
	{
		(*i)++;
		return (ft_itoa(getpid()));
	}
	if (s[*i] == '0')
	{
		(*i)++;
		return (dup_program_name(shell));
	}
	if (s[*i] == '*' || s[*i] == '@' || s[*i] == '#' || s[*i] == '-'
		|| s[*i] == '!')
	{
		(*i)++;
		return (ft_strdup("")); /* vacíos en tu diseño */
	}
	if (ft_isdigit((unsigned char)s[*i]))
	{
		(*i)++;
		return (ft_strdup("")); /* $1..$9 -> vacío en tu diseño */
	}
	return (NULL);
}

static char	*expand_name(const char *s, int *i, t_shell *shell)
{
	int		start;
	char	*name;
	char	*val;

	if (!(ft_isalpha((unsigned char)s[*i]) || s[*i] == '_'))
		return (NULL);
	start = *i;
	while (ft_isalnum((unsigned char)s[*i]) || s[*i] == '_')
		(*i)++;
	name = ft_substr(s, start, *i - start);
	if (!name)
		return (NULL);
	val = (char *)get_env_value(shell, name);
	free(name);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

/* ========= NUEVO: ${VAR} básico =========
   - Soporta: ${VAR}, ${?}, ${$}, ${0}, ${DIGITOS} (vacío)
   - Si falta '}', devuelve literal "${" consumiendo solo '{'
*/
static char	*expand_braced(const char *s, int *i, t_shell *shell)
{
	int		j;
	char	*name;
	char	*val;
	int		start;

	if (s[*i] != '{')
		return (NULL);
	j = *i + 1; /* apunta al primer char tras '{' */
	/* ${} -> vacío (consumimos todo si hay '}') */
	if (s[j] == '}')
	{
		*i = j + 1;
		return (ft_strdup(""));
	}
	/* ${?} / ${$} / ${0} / ${<digits>} */
	if (s[j] == '?')
	{
		j++;
		if (s[j] == '}')
		{
			*i = j + 1;
			return (ft_itoa(shell->exit_status));
		}
		(*i)++;
		return (ft_strdup("${")); /* degradado literal */
	}
	if (s[j] == '$')
	{
		j++;
		if (s[j] == '}')
		{
			*i = j + 1;
			return (ft_itoa(getpid()));
		}
		(*i)++;
		return (ft_strdup("${"));
	}
	if (s[j] == '0')
	{
		j++;
		if (s[j] == '}')
		{
			*i = j + 1;
			return (dup_program_name(shell));
		}
		(*i)++;
		return (ft_strdup("${"));
	}
	if (ft_isdigit((unsigned char)s[j]))
	{
		while (ft_isdigit((unsigned char)s[j]))
			j++;
		if (s[j] == '}')
		{
			*i = j + 1;
			return (ft_strdup(""));
		}
		(*i)++;
		return (ft_strdup("${"));
	}
	/* ${VAR_NAME} */
	if (ft_isalpha((unsigned char)s[j]) || s[j] == '_')
	{
		start = j;
		while (ft_isalnum((unsigned char)s[j]) || s[j] == '_')
			j++;
		if (s[j] != '}')
		{
			(*i)++; /* consumimos solo '{' para emitir "${" y dejar continuar */
			return (ft_strdup("${"));
		}
		name = ft_substr(s, start, j - start);
		if (!name)
			return (NULL);
		val = (char *)get_env_value(shell, name);
		free(name);
		*i = j + 1; /* saltamos la '}' */
		if (!val)
			return (ft_strdup(""));
		return (ft_strdup(val));
	}
	/* Cualquier otro patrón: tratamos como literal "${" */
	(*i)++;
	return (ft_strdup("${"));
}

/* ========= CORREGIDA: respeta $"..." y $'...' (descarta el '$') =========
   Precondición: expand_value se llama con *i en el primer carácter TRAS '$'.
   Casos:
   - $"...": quitamos el '$' y dejamos que el parser trate las comillas.
   - $'...': idem (sin implementar ANSI-C escapes).
*/
char	*expand_value(const char *s, int *i, t_shell *shell)
{
	char	*br;
	char	*sp;
	char	*nm;

	if (!s)
		return (ft_strdup(""));
	/* Fin de cadena tras '$' -> literal "$" */
	if (!s[*i])
		return (ft_strdup("$"));
	/* ${...} antes que el resto */
	if (s[*i] == '{')
	{
		br = expand_braced(s, i, shell);
		if (br)
			return (br);
		/* si no devolvió, seguimos con flujo normal */
	}
	/* Especiales: ?, $$, 0, dígitos y otros que ya defines */
	sp = expand_specials(s, i, shell);
	if (sp)
		return (sp);
	/* NUEVO: si lo siguiente es una comilla, descarta el '$' */
	if (s[*i] == '"' || s[*i] == '\'')
		return (ft_strdup("$"));
	/* Nombre de variable clásico */
	nm = expand_name(s, i, shell);
	if (nm)
		return (nm);
	/* Por defecto: NO consumir el siguiente char → conserva "$" literal */
	return (ft_strdup("$"));
}
