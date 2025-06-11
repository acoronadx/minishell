/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:28:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <asm-generic/signal-defs.h>
#include <bits/sigaction.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

#include "includes/env.h"
#include "includes/ast.h"
#include "includes/lexer.h"
#include "includes/expand.h"

typedef struct s_command
{
	t_token     *args;      // Lista de argumentos (cada uno con value, quoted, etc)
	int         builtin;    // Si es un builtin o no (para exec)
	struct s_command *next;
}   t_command;

typedef struct s_shell
{
	t_env   *env;
	int     exit_status;
	// Otros campos...
}   t_shell;


void	print_usage(const char *invalid);
void	print_help(void);
void	print_version(void);
void	setup_signals(void);
void	run_interactive(char **envp);
void	execute_one_command(const char *command, char **envp);
void	execute_script(const char *script_file, char **envp);
int		ft_quotes_closed(const char *line);
void	handle_sigint(int sig);
void	setup_signals(void);
void	init_shell(void);
void	cleanup_shell(void);
void	parse_and_execute(const char *line, char **envp);
