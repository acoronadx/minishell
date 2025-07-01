/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:17:25 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/30 18:38:14 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"  // t_shell, t_token_type
# include "env.h"        // t_env

/*
** expand_variables.c
** Recorre todos los tokens del shell y aplica expansión.
** Puede modificar tokens in-place o reemplazarlos.
*/
int		expand_variables(t_shell *shell);
/*
** Expande una sola cadena, devolviendo una nueva cadena con las expansiones.
*/
char	*expand_token(const char *str, t_shell *shell);

char	*expand_value(const char *str, int *i, t_shell *shell);

/*
** handle_dollar.c
** Funciones para gestionar la expansión de variables tipo $VAR.
*/
char	*find_var(t_env *env, char *name);
int		handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell);

/*
** calculate_expand_len.c
** Calcula la longitud total necesaria para la expansión de una cadena.
*/
size_t	calculate_expanded_len(const char *str, t_shell *shell);

/*
** handle_expand_cases.c
** Funciones para calcular longitudes específicas de expansiones especiales.
*/
size_t	handle_pid_len(void);
size_t	handle_exit_status_len(t_shell *shell);
size_t	handle_dollar_len(const char *str, int *i, t_shell *shell);
char	*get_program_name_str(t_shell *shell);

/*
** expand_tilde.c
** Funciones para manejar la expansión del tilde (~).
*/
int		copy_tilde(const char *str, int *i, char *res, int *j, t_shell *shell);
char	*expand_tilde_internal(const char *str, t_shell *shell);
size_t	get_tilde_prefix_len(const char *str);

#endif
