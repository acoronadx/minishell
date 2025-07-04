/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:09:44 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 11:53:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_quotes_closed(const char *line)
{
	int	i;
	int	state;

	i = 0;
	state = 0;
	while (line[i])
	{
		if (!state && (line[i] == '\'' || line[i] == '"'))
			state = line[i];
		else if (state == '\'' && line[i] == '\'')
			state = 0;
		else if (state == '"' && line[i] == '\\' &&
			(line[i + 1] == '"' || line[i + 1] == '\\' || line[i + 1] == '`'))
			i++;
		else if (state == '"' && line[i] == '"')
			state = 0;
		else if (!state && line[i] == '\\' && line[i + 1])
			i++;
		i++;
	}
	return (state == 0);
}

char	*read_full_line(t_shell *shell)
{
	char	*next;

	setup_prompt_signals();
	shell->line = readline("\001\033[1;35m\002minishell$ \001\033[0m\002");
	if (!shell->line)
		return (NULL);
	while (!ft_quotes_closed(shell->line))
	{
		setup_prompt_signals();
		next = readline("> ");
		if (!next)
		{
			free(shell->line);
			shell->line = NULL;
			return (NULL);
		}
		shell->line = ft_strjoin_free_s1(shell->line, "\n");
		if (!shell->line)
		{
			free(next);
			return (NULL);
		}
		shell->line = ft_strjoin_free_s1(shell->line, next);
		free(next);
	}
	return (shell->line);
}

/*
** Bucle principal interactivo del minishell.
** Lee, ejecuta y libera cada línea.
** Imprime "exit" si recibe EOF (Ctrl-D), igual que bash.
*/

void	run_interactive(t_shell *shell)
{
	shell->is_interactive = 1;
	while (1)
	{
		setup_prompt_signals(); // <-- Instala handlers antes de cada readline
		if (shell->line)
		{
			free(shell->line);
			shell->line = NULL;
		}
		shell->line = read_full_line(shell);
		if (!shell->line)
			break;
		if (*(shell->line))
		{
			add_history(shell->line);
			setup_ignore_signals(); // Ignora SIGINT/SIGQUIT mientras esperas hijos
			if (shell->tokens)
			{
				free_token_list(shell->tokens);
				shell->tokens = NULL;
			}
			shell_exec(shell);
			setup_prompt_signals(); // Vuelve a instalar handlers tras esperar a los hijos
		}
		cleanup_loop(shell);
		if (shell->should_exit)
			break;
	}
	write(1, "exit\n", 5);
	rl_clear_history();
}


/************************************************************
** Minishell - Input Reading with Quote Handling
** Entrada y gestión de comillas en minishell
**
** ENGLISH:
** - ft_quotes_closed: Checks if all single and double quotes in a line are closed.
**   Returns 1 if they are, 0 if not. Used to detect unterminated quotes so the shell
**   can keep prompting the user for more input (like bash).
** - read_full_line: Reads a full line from the user using readline.
**   If the user input has unclosed quotes, keeps reading additional lines with a
**   secondary prompt ('> '), appending them to the previous line, until all quotes
**   are closed or the user signals EOF (Ctrl+D). Returns the full line or NULL on EOF.
**   Handles memory cleanup safely if an error or EOF occurs.
** - run_interactive: Main interactive loop. Reads a line using read_full_line, adds it
**   to history and executes it if not empty, then frees it. Exits and prints "exit"
**   if EOF (Ctrl+D) is received, just like bash.
**
** - This system ensures that lines with unclosed quotes are handled gracefully,
**   mimicking bash behavior and improving user experience.
**
** ESPAÑOL:
** - ft_quotes_closed: Comprueba si todas las comillas simples y dobles de una línea
**   están cerradas. Devuelve 1 si lo están, 0 si no. Se usa para detectar comillas
**   sin cerrar y así poder seguir pidiendo input al usuario (como hace bash).
** - read_full_line: Lee una línea completa del usuario usando readline.
**   Si hay comillas sin cerrar, sigue pidiendo líneas nuevas con un prompt secundario
**   ('> '), concatenándolas a la anterior, hasta que todas las comillas estén cerradas
**   o el usuario pulse EOF (Ctrl+D). Devuelve la línea completa o NULL en EOF.
**   Gestiona la memoria correctamente en caso de error o EOF.
** - run_interactive: Bucle interactivo principal. Lee una línea con read_full_line,
**   la añade al historial y la ejecuta si no está vacía, y luego la libera.
**   Sale e imprime "exit" si recibe EOF (Ctrl+D), igual que bash.
**
** - Este sistema asegura que las líneas con comillas sin cerrar se gestionen de forma
**   elegante, imitando el comportamiento de bash y mejorando la experiencia de usuario.
************************************************************/


/************************************************************
** Minishell - Main Interactive Loop / Bucle principal interactivo
**
** ENGLISH:
** - Reads user input using readline in each iteration.
** - If readline returns NULL (when the user presses Ctrl+D on an empty line),
**   the loop ends, "exit" is printed, and the program terminates, just like bash.
** - If the line is not empty, the input is added to history and the command is
**   executed.
** - Frees the memory used by the line on each cycle.
** - Note: Signal handling (Ctrl+C, Ctrl+\) is performed in handlers set at the
**   start of the program, not inside this loop.
**
** ESPAÑOL:
** - Lee la entrada del usuario usando readline en cada iteración.
** - Si readline devuelve NULL (lo que ocurre al pulsar Ctrl+D en una línea vacía),
**   el bucle termina, se imprime "exit" y el programa finaliza, igual que bash.
** - Si la línea no está vacía, añade el input al historial y ejecuta el comando.
** - Libera la memoria de la línea en cada ciclo.
** - Nota: El manejo de señales (Ctrl+C, Ctrl+\) se realiza en los handlers
**   configurados al inicio del programa y no dentro de este bucle.
************************************************************/
