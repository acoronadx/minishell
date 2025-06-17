/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_simple_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:05:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 06:11:30 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** parse_group:
** Analiza una agrupación/subshell ( ... ).
** - Avanza el token '('.
** - Llama recursivamente a parse_background (para parsear el contenido).
** - Espera que el siguiente token sea ')'.
** - Si todo va bien, crea un nodo AST de tipo N_SUBSHELL que tiene como hijo izquierdo el subárbol parseado.
** - Si falta el ')', devuelve NULL (error de sintaxis).
*/
t_ast	*parse_group(t_token **cur)
{
	t_ast	*sub;
	t_ast	*node;

	next_token(cur);
	sub = parse_background(cur);
	if (!(*cur) || (*cur)->type != T_RPAREN)
		return (NULL);
	next_token(cur);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = N_SUBSHELL;
	node->argv = NULL;
	node->filename = NULL;
	node->redir_type = 0;
	node->left = sub;
	node->right = NULL;
	return (node);
}

/*
** create_command_node:
** Crea un nodo AST de tipo N_COMMAND.
** - argv: vector de argumentos (comando y sus argumentos).
** - redir_head: lista enlazada de nodos de redirección (puede ser NULL).
** - Inicializa los campos filename y redir_type a NULL/0 porque sólo se usan en N_REDIR.
** - El campo right apunta a la lista de redirecciones asociadas al comando.
*/
t_ast	*create_command_node(char **argv, t_ast *redir_head)
{
	t_ast	*cmd;

	cmd = malloc(sizeof(t_ast));
	if (!cmd)
		return (NULL);
	cmd->type = N_COMMAND;
	cmd->argv = argv;
	cmd->filename = NULL;
	cmd->redir_type = 0;
	cmd->left = NULL;
	cmd->right = redir_head;
	return (cmd);
}

/*
** make_redir_node:
** Crea un nodo de redirección a partir de dos tokens:
** - redir: el token de tipo redirección (>, >>, <, etc.)
** - file: el token con el nombre del archivo objetivo.
** - Devuelve NULL en caso de error de memoria.
** - El nodo se enlazará en la lista de redirecciones del comando.
*/
t_ast	*make_redir_node(t_token *redir, t_token *file)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = N_REDIR;
	node->filename = ft_strdup(file->value);
	node->redir_type = redir->type;
	node->argv = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

/*
** parse_redirections:
** Analiza y encadena todas las redirecciones consecutivas en la lista de tokens.
** - Por cada redirección encontrada, avanza el puntero y crea un nodo con make_redir_node.
** - Encadena los nodos de redirección usando el campo right.
** - redir_head: apunta al primer nodo de la lista de redirecciones.
** - redir_tail: apunta al último nodo, para poder añadir nuevos al final.
** - Si ocurre un error (token de redirección sin archivo, o malloc), devuelve 0.
** - Si todo va bien, devuelve 1.
*/
int	parse_redirections(t_token **cur, t_ast **redir_head, t_ast **redir_tail)
{
	t_token	*redir;
	t_ast	*tmp;

	while (*cur && is_redirection(*cur))
	{
		redir = *cur;
		next_token(cur);
		if (!*cur || (*cur)->type != T_WORD)
			return (0);
		tmp = make_redir_node(redir, *cur);
		if (!tmp)
			return (0);
		if (!*redir_head)
			*redir_head = tmp;
		else
			(*redir_tail)->right = tmp;
		*redir_tail = tmp;
		next_token(cur);
	}
	return (1);
}

/*
** parse_arguments:
** Crea y rellena un array de strings (argv) a partir de tokens T_WORD consecutivos.
** - Usa add_argument (archivo aparte) para gestionar el array dinámicamente.
** - El bucle termina al no haber más T_WORD.
** - Termina argv con un puntero NULL.
** - Devuelve el array argv o NULL en caso de error.
*/
char	**parse_arguments(t_token **cur)
{
	int		size;
	int		argc;
	char	**argv;

	size = 16;
	argc = 0;
	argv = malloc(sizeof(char *) * size);
	if (!argv)
		return (NULL);
	while (*cur && (*cur)->type == T_WORD)
	{
		argv = add_argument(argv, &argc, &size, (*cur)->value);
		if (!argv)
			return (NULL);
		next_token(cur);
	}
	argv[argc] = NULL;
	return (argv);
}
