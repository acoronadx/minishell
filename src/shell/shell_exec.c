/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:49:39 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/05 15:12:22 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expand.h"
#include "parser.h"
#include "ast.h"

int	shell_exec(t_shell *shell)
{
	shell->tokens = lexer(shell->line);
	if (!shell->tokens)
	{
		shell->exit_status = 2; // Error de sintaxis como Bash
		return (2);
	}
	prueba_lexer(shell);
	expand_variables(shell);
	shell->ast = build_ast(shell->tokens);
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	if (!shell->ast)
	{
		shell->exit_status = 2; // Error de sintaxis como Bash
		return (2);
	}
	execute_ast(shell->ast, shell);
	return (shell->exit_status);
}

char	*token_type_str(t_token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_OR)
		return ("OR");
	if (type == T_AND)
		return ("AND");
	if (type == T_BG)
		return ("BG");
	if (type == T_SEMI)
		return ("SEMI");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_APPEND)
		return ("APPEND");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_REDIR_ERR)
		return ("REDIR_ERR");
	if (type == T_APPEND_ERR)
		return ("APPEND_ERR");
	if (type == T_REDIR_ALL)
		return ("REDIR_ALL");
	if (type == T_APPEND_ALL)
		return ("APPEND_ALL");
	if (type == T_FORCE_OUT)
		return ("FORCE_OUT");
	if (type == T_HEREDOC_STR)
		return ("HEREDOC_STR");
	if (type == T_DUP_IN)
		return ("DUP_IN");
	if (type == T_DUP_OUT)
		return ("DUP_OUT");
	if (type == T_LPAREN)
		return ("LPAREN");
	if (type == T_RPAREN)
		return ("RPAREN");
	if (type == T_LBRACE)
		return ("LBRACE");
	if (type == T_RBRACE)
		return ("RBRACE");
	if (type == T_EQUAL)
		return ("EQUAL");
	if (type == T_UNKNOWN)
		return ("UNKNOWN");
	return ("OTHER");
}

static char	*quote_type_str(t_quote q)
{
	if (q == NO_QUOTE)
		return ("NO_QUOTE");
	if (q == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	if (q == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("OTHER");
}

void	prueba_lexer(t_shell *shell)
{
	t_token	*tok;
	int		i;

	tok = shell->tokens;
	i = 0;
	if (!tok)
	{
		printf("No tokens found.\n");
		return ;
	}
	while (tok)
	{
		printf("Token %d:\n", i);
		printf("  value: \"%s\"\n", tok->value);
		printf("  type: %s\n", token_type_str(tok->type));
		printf("  quote: %s\n", quote_type_str(tok->quoted));
		tok = tok->next;
		i++;
	}
}

void	prueba_env(t_shell *shell)
{
	t_env	*env;
	int		i;

	env = shell->env;
	i = 0;
	if (!env)
	{
		printf("No environment variables found.\n");
		return ;
	}
	printf("Current environment variables:\n");
	while (env)
	{
		printf("🧪 [%d] key = \"%s\" | value = \"%s\" | exported = %d\n",
			i,
			env->key ? env->key : "(null)",
			env->value ? env->value : "(null)",
			env->exported);
		env = env->next;
		i++;
	}
}

void	prueba_expansion(t_shell *shell)
{
	t_token	*tok;
	int		i;

	tok = shell->tokens;
	i = 0;
	if (!tok)
	{
		printf("No tokens found for expansion.\n");
		return ;
	}
	printf("Prueba de expansión:\n");
	while (tok)
	{
		printf("Token %d:\n", i);
		printf("  value: \"%s\"\n", tok->value ? tok->value : "(null)");
		printf("  type: %s\n", token_type_str(tok->type));
		printf("  quote: %s\n", quote_type_str(tok->quoted));
		tok = tok->next;
		i++;
	}
	printf("Prompt simulado: ");
	tok = shell->tokens;
	while (tok)
	{
		if (tok->value)
			printf("%s ", tok->value);
		tok = tok->next;
	}
	printf("\n");
}

#include <stdio.h>

static void print_indent(int level)
{
    while (level--)
        printf("  ");
}

void prueba_ast(t_shell *shell)
{
    if (!shell || !shell->ast)
    {
        printf("No hay AST que mostrar.\n");
        return;
    }
    printf("======= AST DEBUG =======\n");
    print_ast_debug(shell->ast, 0);
    printf("=========================\n");
}
#include <stdio.h>

void print_ast_debug(t_ast *node, int level)
{
    if (!node)
        return;

    print_indent(level);
    printf("Node type: ");

    if (node->type == N_COMMAND)
    {
        printf("N_COMMAND\n");
        print_indent(level);
        printf("  argv:");
        if (node->cmd.argv)
        {
            for (int i = 0; node->cmd.argv[i]; i++)
                printf(" \"%s\"", node->cmd.argv[i]);
        }
        printf("\n");

        if (node->cmd.redirections)
        {
            print_indent(level);
            printf("  redirections:\n");
            t_ast *redir = node->cmd.redirections;
            while (redir)
            {
                print_ast_debug(redir, level + 2);
                redir = redir->bin.right; // asumimos lista ligada con bin.right
            }
        }
    }
    else if (node->type == N_REDIR)
    {
        printf("N_REDIR\n");
        print_indent(level);
        printf("  redir_type: %d\n", node->redir.redir_type);
        print_indent(level);
        printf("  filename: \"%s\"\n", node->redir.filename);
        print_indent(level);
        printf("  delimiter: \"%s\"\n", node->redir.delimiter ? node->redir.delimiter : "NULL");
        print_indent(level);
        printf("  redir_fd: %d\n", node->redir.redir_fd);
    }
    else if (node->type == N_PIPE || node->type == N_AND
             || node->type == N_OR || node->type == N_SEQUENCE
             || node->type == N_BACKGROUND)
    {
        const char *type_str = (node->type == N_PIPE) ? "N_PIPE" :
                              (node->type == N_AND) ? "N_AND" :
                              (node->type == N_OR) ? "N_OR" :
                              (node->type == N_SEQUENCE) ? "N_SEQUENCE" :
                              (node->type == N_BACKGROUND) ? "N_BACKGROUND" : "UNKNOWN";

        printf("%s\n", type_str);
        print_indent(level);
        printf("  left:\n");
        print_ast_debug(node->bin.left, level + 1);
        print_indent(level);
        printf("  right:\n");
        print_ast_debug(node->bin.right, level + 1);
    }
    else if (node->type == N_SUBSHELL)
    {
        printf("N_SUBSHELL\n");
        print_indent(level);
        printf("  child:\n");
        print_ast_debug(node->subshell.child, level + 1);

        if (node->subshell.redirections)
        {
            print_indent(level);
            printf("  redirections:\n");
            t_ast *redir = node->subshell.redirections;
            while (redir)
            {
                print_ast_debug(redir, level + 2);
                redir = redir->bin.right;
            }
        }
    }
    else
    {
        printf("Unknown node type %d\n", node->type);
    }
}
