/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 06:03:09 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <asm-generic/signal-defs.h>
#include <bits/sigaction.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

#include "includes/env.h"
#include "includes/ast.h"
#include "includes/lexer.h"
#include "includes/parser.h"
#include "includes/expand.h"

typedef struct s_shell
{
	t_env		*env;          // Variables de entorno
	char		*line;         // Línea actual recibida por readline
	t_token		*tokens;       // Lista de tokens de la línea actual
	t_ast		*ast;          // Árbol de sintaxis actual
	int			exit_status;   // Último status de ejecución
	int			is_interactive; // 1 si es interactivo, 0 si no
	int			is_script;      // 1 si ejecuta script, 0 si no
	int			is_one_command; // 1 si está en modo -c, 0 si no
	int			should_exit;    // 1 si se debe salir del shell,
	// Puedes añadir más campos si necesitas (por ejemplo, flags de modo, etc)
}	t_shell;

void	init_shell(t_shell *shell, char **envp);
void	print_usage(const char *invalid);
void	print_help(void);
void	print_version(void);
int		handle_help_version(int argc, char **argv);
void	setup_signals(void);
void	run_interactive(t_shell *shell);
void	execute_one_command(const char *command, char **envp);
void	execute_script(const char *filename, t_shell *shell);
int		is_builtin(char *cmd);
int		execute_builtin(char **argv, t_shell *shell);
int		execute_ast(t_ast *node, t_shell *shell);
int		ft_quotes_closed(const char *line);
void	handle_sigint(int sig);
void	setup_signals(void);
void	free_token_list(t_token *tok);
void	clean_line(char **line);
void	cleanup_loop(t_shell *shell);
void	cleanup_shell(t_shell shell);
t_ast	*parser_line(t_shell *shell);
void	parse_and_execute(t_shell *shell);

#endif



/*
Las flags de modo en el contexto de un shell (como bash o tu minishell) son variables o campos que indican en qué “modo” está funcionando el shell, o qué opciones/funcionalidades están activas en ese momento.
¿Qué tipos de “modos” puede tener un shell?

Algunos ejemplos de modos y flags típicas:

    Interactivo: ¿El shell está mostrando un prompt y esperando comandos del usuario?
    Script: ¿El shell está ejecutando comandos desde un fichero?
    Comando único: ¿El shell está ejecutando solo un comando (por ejemplo, minishell -c "ls")?
    Modo depuración: ¿Está activada la salida de debug?
    Modo sin historial: ¿Se debe guardar el historial de comandos?
    Modo heredoc: ¿Está leyendo una entrada heredoc?
    Modo subshell: ¿Se está ejecutando un comando dentro de un subshell (paréntesis)?

¿Cómo se representan estas flags?

Con campos tipo int, bool o char en tu struct principal (por ejemplo, en tu t_shell):
C

typedef struct s_shell
{
    t_env   *env;
    char    *line;
    t_token *tokens;
    t_ast   *ast;
    int     exit_status;
    int     is_interactive; // 1 si es interactivo, 0 si no
    int     is_script;      // 1 si ejecuta script, 0 si no
    int     is_one_command; // 1 si está en modo -c, 0 si no
    // ...otros flags de modo
}   t_shell;

O usando un campo bitfield o un solo entero y defines para ahorrar espacio:
C

#define MODE_INTERACTIVE  1
#define MODE_SCRIPT       2
#define MODE_ONE_COMMAND  4

typedef struct s_shell
{
    // ...
    int mode_flags; // cada bit representa un modo
} t_shell;

¿Para qué sirven?

    Para que tu shell sepa cómo comportarse en cada situación.
        Por ejemplo: solo muestra el prompt si está en modo interactivo.
        Si está en modo script, no guarda en historial.
        Si está en modo heredoc, puede cambiar el manejo de señales.
    Para que puedas escribir código más limpio y mantenible usando if (shell->is_interactive) o similar.

Resumen

    Son campos en tu struct que indican el “modo” de funcionamiento del shell.
    Te ayudan a adaptar el comportamiento del shell según cómo fue llamado o qué está ejecutando.

*/