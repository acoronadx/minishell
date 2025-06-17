/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:26:15 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 08:44:25 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Variable global utilizada para indicar si se ha recibido una señal.
// 'volatile' previene optimizaciones del compilador que podrían causar problemas
// y 'sig_atomic_t' es seguro de modificar dentro de un handler de señal.
volatile sig_atomic_t	g_signal = 0;

/*
** Handler para SIGINT (Ctrl+C)
** Se llama automáticamente cuando el usuario pulsa Ctrl+C.
** - Marca que la señal se recibió (g_signal = 1)
** - Escribe una nueva línea y vuelve a mostrar el prompt "minishell$ "
** - Limpia la línea actual de readline y actualiza el display.
*/
void	handle_sigint(int sig)
{
	(void)sig;                       // Evita warning por parámetro no usado
	g_signal = 1;                    // Marca que se recibió SIGINT
	write(1, "\nminishell$ ", 12);   // Muestra prompt en nueva línea
	rl_on_new_line();                // Notifica a readline de nueva línea
	rl_replace_line("", 0);          // Borra la línea que el usuario escribía
	rl_redisplay();                  // Redibuja el prompt limpio
}

/*
** Handler para SIGQUIT (Ctrl+\)
** Se llama automáticamente cuando el usuario pulsa Ctrl+\.
** No hace nada aquí, así que la señal es ignorada.
*/
void	handle_sigquit(int sig)
{
	(void)sig; // Evita warning por parámetro no usado
}

/*
** Configura el manejo de señales para el minishell.
** - SIGINT (Ctrl+C): llama a handle_sigint
** - SIGQUIT (Ctrl+\): llama a handle_sigquit (no hace nada)
** SA_RESTART: reinicia ciertas syscalls interrumpidas por señales.
*/
void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, 0);

	sa.sa_handler = handle_sigquit;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, 0);
}
/************************************************************
** Minishell - Signal Handling / Gestión de señales
**
** ENGLISH:
** - Ctrl+C (SIGINT): Interrupts the current input, clears the line, and shows the
**   prompt again, without closing the minishell. The handler writes a new line
**   and uses readline functions to clear and refresh the prompt.
**
** - Ctrl+\ (SIGQUIT): Is ignored, preventing the minishell from terminating or
**   showing core dump messages as bash does.
**
** - Ctrl+D (EOF): Is NOT a signal. When the user presses Ctrl+D on an empty line,
**   readline returns NULL, which is detected in the main loop of the minishell to
**   print "exit" and terminate the process, imitating bash behavior.
**
** Note: All signals are set at the start of the program and only functions allowed
** by the Norm and the project guidelines are used.
**
** ESPAÑOL:
** - Ctrl+C (SIGINT): Interrumpe el input actual, limpia la línea y muestra el prompt
**   de nuevo, sin cerrar el minishell. El handler escribe una nueva línea y utiliza
**   funciones de readline para limpiar y refrescar el prompt.
**
** - Ctrl+\ (SIGQUIT): Se ignora, evitando que el minishell termine o muestre mensajes
**   de core dump como en bash.
**
** - Ctrl+D (EOF): NO es una señal. Cuando el usuario pulsa Ctrl+D en una línea vacía,
**   readline devuelve NULL, que se detecta en el bucle principal del minishell para
**   imprimir "exit" y terminar el proceso, imitando el comportamiento de bash.
**
** Nota: Todas las señales se configuran al inicio del programa y sólo se usan funciones
** permitidas por la Norme y el enunciado.
************************************************************/