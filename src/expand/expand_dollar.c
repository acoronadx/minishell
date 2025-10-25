/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 16:42:07 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

/*
static char	*remove_empty_quotes_post_expand(const char *src)
{
	char	*clean;
	int		len;

	len = ft_strlen(src);
	if (len < 2)
		return (ft_strdup(src));
	if ((src[len - 2] == '"' && src[len - 1] == '"')
		|| (src[len - 2] == '\'' && src[len - 1] == '\''))
	{
		clean = ft_substr(src, 0, len - 2);
		if (!clean)
			return (NULL);
		return (clean);
	}
	return (ft_strdup(src));
}
*/
/*
static char	*expand_special_vars(const char *str, int *i, t_shell *shell)
{
	char	*res;

	res = NULL;
	if (str[*i] == '?')
	{
		res = ft_itoa(shell->exit_status);
		(*i)++;
	}
	else if (str[*i] == '0')
	{
		res = get_program_name_str(shell);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		res = ft_itoa(getpid());
		(*i)++;
	}
	return (res);
}

static char	*expand_named_var(const char *str, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	start = *i;
	while (str[*i] == '_' || ft_isalnum(str[*i]))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	if (!name)
		return (NULL);
	value = find_var(shell->env, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
*/

// Modificar expand_value para manejar $""
char    *expand_value(const char *str, int *i, t_shell *shell)
{
    int     start_var_name;

    // ... (código existente para $?, $0, $$)

    if (ft_isdigit(str[*i])) // Manejo de parámetros posicionales ($1, $2, etc.)
    {
        // Si es $0, ya fue manejado. Esto es para $1, $2, etc.
        // Estos se expanden a "" si no existen
        (*i)++; // Avanza el puntero para saltar el dígito
        return (ft_strdup(""));
    }
    
    // --- NUEVA LÓGICA PARA MANEJAR `$` seguido de comillas o fin de cadena ---
    // Si $ está al final de la cadena O seguido de comilla, Bash lo evalúa a cadena vacía
    if (!str[*i] || str[*i] == '"' || str[*i] == '\'')
    {
        // Si `$""`, el `handle_dollar` ya avanzó `i` al inicio de `""`.
        // Si `$` solo, `i` está en '\0'.
        // En ambos casos, expandir a cadena vacía.
        return (ft_strdup(""));
    }
    // --- FIN DE NUEVA LÓGICA ---

    if (str[*i] == '_' || ft_isalpha(str[*i])) // Manejo de variables con nombre
    {
        start_var_name = *i;
        while (str[*i] == '_' || ft_isalnum(str[*i]))
            (*i)++;
        char *name = ft_substr(str, start_var_name, *i - start_var_name);
        if (!name) return (NULL);
        char *value = find_var(shell->env, name); // find_var devuelve "" si no existe
        free(name);
        return (ft_strdup(value)); // Duplicar para que el caller pueda liberarlo
    }
    
    // Si solo es $, o $ seguido de un caracter que no forma parte de un nombre de var
    // Ej: `$` o `$#` (pero `#` no es un identificador válido para una variable)
    // Bash expande a "$" si no es un caso especial ($?, $$, $0, nombre_valido)
    // y no está seguido de un separador.
    // Sin embargo, si está seguido de un separador o fin de palabra, es complejo.
    // Simplificación: si no es ninguna de las anteriores, lo dejamos como "$"
    // y el `quote removal` se encargará de cualquier comilla adyacente.
    // Ojo: En `$#`, bash lo expande a la cantidad de argumentos. Esto no está implementado.
    // Por ahora, para un `$`, simplemente lo pasamos como literal.
    // ¡Ojo! El `i` no se avanzó si es un `$#` (por ejemplo)
    (*i)++; // Asegura que se avanza el puntero si no se hizo en las ramas anteriores
    return (ft_strdup("$")); // Si no es $?, $$, $0, o variable válida, se queda como literal "$"
}

int handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell)
{
    char    *expanded;
    int     len;

    (*i)++; // saltar el $
    expanded = expand_value(str, i, shell);
    if (!expanded)
        return (-1);
    len = ft_strlen(expanded);
    ft_memcpy(res + j, expanded, len);
    free(expanded);
    return (len);
}
