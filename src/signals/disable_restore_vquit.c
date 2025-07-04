/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disable_restore_vquit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:13:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/03 19:30:21 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static struct termios	g_old_term;
static struct termios	g_new_term;

/*
** Desactiva la señal SIGQUIT (Ctrl-\) en el terminal.
** Guarda la configuración anterior para poder restaurarla.
** Idempotente: Puede llamarse varias veces sin efectos adversos.
*/
void	disable_vquit(void)
{
	tcgetattr(0, &g_old_term);
	g_new_term = g_old_term;
	g_new_term.c_cc[VQUIT] = 0;
	tcsetattr(0, TCSANOW, &g_new_term);
}

/*
** Restaura la visualización de caracteres de control (ECHOCTL)
** y el comportamiento por defecto de SIGQUIT (Ctrl-\).
** Idempotente: Puede llamarse múltiples veces de forma segura,
** incluso si no se llamó previamente a disable_vquit().
*/
void	restore_vquit(void)
{
	struct termios term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	signal(SIGQUIT, SIG_DFL);
}
