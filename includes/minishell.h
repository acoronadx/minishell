/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:11:47 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <errno.h>
#include <linux/limits.h>
#include <limits.h>
#include "libft.h"
#include "env.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "exec.h"
#include "builtins.h"
#include "signals.h"

typedef struct s_ast t_ast;
typedef struct s_shell t_shell;
typedef struct s_token t_token;

typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
	int             exported;
}                   t_env;

typedef struct s_shell
{
        t_env           *env;
        char            *line;
        t_token         *tokens;
        t_ast           *ast;
        int             exit_status;
		int				in_child;  // 0 por defecto; en hijos se pone a 1 antes de ejecutar AST
        int             is_interactive;
        int             is_script;
        int             is_one_command;
        int             should_exit;
        char            *program_name;
        int             pid; // PID del shell actual (¡Correcto! Usado para $$, $BASHPID)
}       t_shell;

extern volatile sig_atomic_t   g_signal;
// Shell modes and flags
int		run_shell_modes(int argc, char **argv, char **envp, t_shell *shell);

// Inicialización y limpieza del shell
int		init_shell_name(t_shell *shell, char **argv);
int		init_env(t_shell *shell, char **envp);
void    cleanup_shell(t_shell *shell);
void    cleanup_loop(t_shell *shell);
int		print_exec_error(t_shell *shell, const char *cmd, int err_code);

// Funciones de ayuda/uso/version
void	print_help(void);
void	print_usage(const char *invalid);
void	print_version(void);
int		handle_help_version(int argc, char **argv);

// Ejecución principal del shell
void	prueba_lexer(t_shell *shell);
void	prueba_env(t_shell *shell);
void	prueba_expansion(t_shell *shell);
void	prueba_ast(t_shell *shell);
void	print_ast_debug(t_ast *node, int level);

int		shell_exec(t_shell *shell);
//void	parse_and_execute(t_shell *shell);
//int		check_syntax(t_ast *ast);

// --- Modos ---
int   run_shell_modes(int argc, char **argv, char **envp, t_shell *shell);
void  run_interactive(t_shell *shell);
void  run_non_interactive(t_shell *shell);   // NUEVO

// --- Lectura de línea ---
char *read_line_interactive(t_shell *shell); // RENOMBRE de read_full_line
// en no-interactivo leeremos con get_next_line o similar (o readline sin prompt)

#endif