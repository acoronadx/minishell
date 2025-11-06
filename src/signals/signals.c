/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:26:15 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 12:07:29 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Única global permitida */
volatile sig_atomic_t	g_signal = 0;

/* -------------------------------------------------------------------------- */
/*  ECHOCTL on/off: muestra los controles como ^C/^\\ si está activo.         */
/*  (No guardamos estado global adicional; simplemente forzamos el bit.)      */
/* -------------------------------------------------------------------------- */
void	set_echoctl(int enable)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &t) == -1)
		return ;
#ifdef ECHOCTL
	if (enable)
		t.c_lflag |= ECHOCTL;
	else
		t.c_lflag &= ~ECHOCTL;
#endif
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

/* Compat con tu código existente */
void	disable_vquit(void)
{
	set_echoctl(0);
}
void	restore_vquit(void)
{
	set_echoctl(1);
}

/* -------------------------------------------------------------------------- */
/*  Handler de SIGINT en el PROMPT:                                           */
/*  - Escribe salto de línea                                                  */
/*  - Limpia la línea de readline y redibuja el prompt                        */
/* 
	- Marca g_signal para que el bucle principal ponga exit_status=130         */
/* -------------------------------------------------------------------------- */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* En el prompt, Ctrl-\ no hace nada */
void	handle_sigquit(int sig)
{
	(void)sig;
	/* nada */
}

/* -------------------------------------------------------------------------- */
/*  Prompt interactivo:                                                       */
/*  - Mostrar ^C/^\ (ECHOCTL activado).                                       */
/*  - SIGINT -> handler que aborta línea.                                     */
/*  - SIGQUIT -> ignorado.                                                    */
/* -------------------------------------------------------------------------- */
void	setup_prompt_signals(void)
{
	set_echoctl(1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/* Padre durante ejecución de comandos: no debe matar al hijo al pulsar ^C/^\ */
void	setup_exec_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* Defaults (modo no-interactivo). También lo puedes usar si quieres resetear. */
void	setup_default_signals(void)
{
	set_echoctl(1); /* ver ^C/^\ en programas */
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* Compat: muchos sitios llaman a setup_signals() al inicio -> prompt. */
void	setup_signals(void)
{
	setup_prompt_signals();
}
