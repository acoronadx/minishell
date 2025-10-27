/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 08:56:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/27 12:07:10 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

/* ÚNICA variable global permitida del proyecto */
extern volatile sig_atomic_t g_signal;

/* Prompt (interactivo): ^C aborta la línea, ^\ se ignora. */
void    setup_prompt_signals(void);

/* En el PADRE mientras se ejecutan comandos: ignorar INT/QUIT. */
void    setup_exec_parent_signals(void);

/* Para modo no-interactivo o para restaurar defaults explícitamente. */
void    setup_default_signals(void);

/* Alias de compatibilidad: inicializa señales de prompt. */
void    setup_signals(void);

/* Handlers del prompt */
void    handle_sigint(int sig);
void    handle_sigquit(int sig);

/* TTY: controlar si el terminal muestra ^C / ^\ (ECHOCTL). */
void    set_echoctl(int enable);

/* Compat: usados en tu código existente (simple envoltorio de set_echoctl) */
void    disable_vquit(void);   /* oculta ^C/^\\ en el prompt si quisieras */
void    restore_vquit(void);   /* vuelve a mostrar ^C/^\\ */

#endif
