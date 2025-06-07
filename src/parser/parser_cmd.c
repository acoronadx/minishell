/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 23:50:47 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 00:06:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *cmd_new(void)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd) return NULL;
    cmd->argv = NULL;
    cmd->redir_in = NULL;
    cmd->redir_out = NULL;
    cmd->append_out = 0;
    cmd->heredoc = 0;
    cmd->next = NULL;
    return cmd;
}

static int count_args(t_token *tok)
{
    int n = 0;
    while (tok && tok->type == T_WORD) {
        n++;
        tok = tok->next;
    }
    return n;
}

static int	parse_redirections(t_cmd *cmd, t_token **tokens)
{
	while (*tokens && ((*tokens)->type == T_REDIR_IN || (*tokens)->type == T_HEREDOC
			|| (*tokens)->type == T_REDIR_OUT || (*tokens)->type == T_REDIR_APPEND))
	{
		t_token_type t = (*tokens)->type;
		*tokens = (*tokens)->next;
		if (!(*tokens) || (*tokens)->type != T_WORD) {
			const char *unexpected = (*tokens) ? (*tokens)->value : "newline";
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", unexpected);
			return (1);
		}
		if (t == T_REDIR_IN)
			cmd->redir_in = strdup((*tokens)->value);
		else if (t == T_HEREDOC) {
			cmd->redir_in = strdup((*tokens)->value);
			cmd->heredoc = 1;
		}
		else if (t == T_REDIR_OUT) {
			cmd->redir_out = strdup((*tokens)->value);
			cmd->append_out = 0;
		}
		else if (t == T_REDIR_APPEND) {
			cmd->redir_out = strdup((*tokens)->value);
			cmd->append_out = 1;
		}
		*tokens = (*tokens)->next;
	}
	return (0);
}

static int	parse_arguments(t_cmd *cmd, t_token **tokens)
{
	int argc = count_args(*tokens);
	if (argc)
	{
		cmd->argv = malloc(sizeof(char *) * (argc + 1));
		if (!cmd->argv)
			return (1);
		int i = 0;
		while (i < argc && *tokens && (*tokens)->type == T_WORD)
		{
			cmd->argv[i] = strdup((*tokens)->value);
			if (!cmd->argv[i])
				return (1);
			i++;
			*tokens = (*tokens)->next;
		}
		cmd->argv[i] = NULL;
	}
	return (0);
}

t_cmd *parse_commands(t_token *tokens)
{
	t_cmd *head = NULL;
	t_cmd *last = NULL;
	while (tokens)
	{
		t_cmd *cmd = cmd_new();
		if (!cmd) {
			free_cmds(head);
			return NULL;
		}
		if (parse_arguments(cmd, &tokens) || parse_redirections(cmd, &tokens)) {
			free_cmds(cmd);
			free_cmds(head);
			return NULL;
		}
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tokens && tokens->type == T_PIPE) {
			if (!tokens->next || tokens->next->type == T_PIPE) {
				const char *unexpected = (tokens->next) ? "|" : "newline";
				fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", unexpected);
				free_cmds(head);
				return NULL;
			}
			tokens = tokens->next;
		}
	}
	return head;
}
