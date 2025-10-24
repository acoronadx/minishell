/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 08:56:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 03:11:28 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

extern volatile sig_atomic_t g_signal;

void  setup_signals(void);
void  setup_ignore_signals(void);
void  setup_prompt_signals(void);
void  setup_default_signals(void);           // <-- NUEVO
void  handle_sigint(int sig);
void  handle_sigquit(int sig);
void  disable_vquit(void);
void  restore_vquit(void);

#endif