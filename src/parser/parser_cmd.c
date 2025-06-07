/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 23:50:47 by codespace         #+#    #+#             */
/*   Updated: 2025/06/07 00:58:17 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "errors.h"

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	return (cmd);
}

static int	count_args(t_token *tok)
{
	int	n;

	n = 0;
	while (tok && tok->type == T_WORD)
	{
		n++;
		tok = tok->next;
	}
	return (n);
}

static int	parse_args(t_cmd *cmd, t_token **tokens)
{
	int	argc;
	int	i;

	argc = count_args(*tokens);
	if (argc)
	{
		cmd->argv = ft_calloc(argc + 1, sizeof(char *));
		if (!cmd->argv)
			return (1);
		i = 0;
		while (i < argc && *tokens && (*tokens)->type == T_WORD)
		{
			cmd->argv[i] = ft_strdup((*tokens)->value);
			if (!cmd->argv[i])
				return (1);
			i++;
			*tokens = (*tokens)->next;
		}
		cmd->argv[i] = NULL;
	}
	return (0);
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;

	head = NULL;
	last = NULL;
	while (tokens)
	{
		cmd = cmd_new();
		if (!cmd)
			return (free_cmds(head));
		if (parse_args(cmd, &tokens) || parse_redirections(cmd, &tokens))
		{
			free_cmds(cmd);
			return (free_cmds(head));
		}
		add_cmd_to_list(&head, &last, cmd);
		if (tokens && tokens->type == T_PIPE)
		{
			if (check_pipe_syntax(tokens, head))
				return (NULL);
			tokens = tokens->next;
		}
	}
	return (head);
}
