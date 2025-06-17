/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_command.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:39:45 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:07:41 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "ast.h"

/*
** is_lparen:
** Devuelve 1 si el token actual es un paréntesis de apertura '('.
** Sirve para detectar el inicio de una agrupación o subshell.
*/
static int	is_lparen(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == T_LPAREN);
}

/*
** add_argument:
** Añade un string (argumento) al array argv, redimensionando si es necesario.
** - argv: el array actual de argumentos.
** - argc: puntero al contador de argumentos.
** - size: puntero al tamaño actual del array.
** - value: string a añadir.
** Devuelve el nuevo array (puede ser el mismo o uno nuevo si se realocó).
*/
char	**add_argument(char **argv, int *argc, int *size, char *value)
{
	char	**tmp;

	if (*argc + 1 >= *size)
	{
		tmp = ft_realloc(argv, (*size) * sizeof(char *),
				(*size) * 2 * sizeof(char *));
		if (!tmp)
			return (NULL);
		argv = tmp;
		*size = (*size) * 2;
	}
	argv[*argc] = ft_strdup(value);
	*argc = *argc + 1;
	return (argv);
}

/*
** parse_simple_command:
** Analiza una secuencia de tokens para construir un AST de comando simple.
** - Si empieza por '(', delega en parse_group (subshell/agrupación).
** - Procesa redirecciones (antes y después de los argumentos).
** - Procesa argumentos (palabras) y los mete en argv.
** - Si no hay argumentos válidos, devuelve la lista de redirecciones.
** - Devuelve un nodo AST de tipo comando, con su argv y la cadena de redirecciones.
*/
t_ast	*parse_simple_command(t_token **cur)
{
	t_ast	*redir_head;
	t_ast	*redir_tail;
	char	**argv;
	t_ast	*cmd;

	if (is_lparen(*cur))
		return (parse_group(cur));
	redir_head = NULL;
	redir_tail = NULL;
	if (!parse_redirections(cur, &redir_head, &redir_tail))
		return (NULL);
	argv = parse_arguments(cur);
	if (!argv)
		return (redir_head);
	if (!parse_redirections(cur, &redir_head, &redir_tail))
	{
		free_strtab(argv);
		return (NULL);
	}
	cmd = create_command_node(argv, redir_head);
	return (cmd);
}

/*
** is_redirection:
** Devuelve 1 si el token actual es algún tipo de redirección.
** Así se detectan tokens como >, >>, <, <<, 2>, &> y variantes.
** Permite separar los tokens de redirección de los de palabra/argumento.
*/

int	is_redirection(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == T_REDIR_IN || tok->type == T_REDIR_OUT
		|| tok->type == T_APPEND || tok->type == T_HEREDOC
		|| tok->type == T_APPEND_ERR || tok->type == T_REDIR_ERR
		|| tok->type == T_REDIR_ALL || tok->type == T_APPEND_ALL
		|| tok->type == T_FORCE_OUT || tok->type == T_DUP_IN
		|| tok->type == T_DUP_OUT || tok->type == T_HEREDOC_STR);
}
