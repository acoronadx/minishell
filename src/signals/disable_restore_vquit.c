/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disable_restore_vquit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:13:14 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 20:29:48 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include "minishell.h"

static struct termios	g_old_term;
static struct termios	g_new_term;

void	disable_vquit(void)
{
	tcgetattr(0, &g_old_term);
	g_new_term = g_old_term;
	g_new_term.c_cc[VQUIT] = 0;
	tcsetattr(0, TCSANOW, &g_new_term);
}

void	restore_vquit(void)
{
	tcsetattr(0, TCSANOW, &g_old_term);
}
