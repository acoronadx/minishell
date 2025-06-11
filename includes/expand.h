/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:27:52 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 16:27:55 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

/*
** Expande las variables presentes en el string 'str' usando el entorno de shell.
** Devuelve una nueva cadena con las variables expandidas.
*/
char	*expand_token(const char *str, t_shell *shell);

/*
** Expande todas las variables de los argumentos de cada comando de la lista.
** Solo expande si el argumento no está entre comillas simples.
*/
void	expand_variables(t_command *cmd_list, t_shell *shell);

/*
** Busca el valor de una variable en la lista enlazada de entorno.
** Devuelve un puntero a value o "" si no existe la variable.
*/
char	*find_var(t_env *env, char *name);

/*
** Expande $? insertando el exit_status en res.
** Devuelve el nuevo índice j tras copiar el valor.
*/
int		handle_exit_status(char *res, int j, int *i, t_shell *shell);

/*
** Expande el nombre de variable a partir de str[*i] y la copia en res.
** Devuelve el nuevo índice j tras copiar el valor.
*/
int		handle_varname(const char *str, int *i, char *res, int j, t_shell *shell);

/*
** Gestiona la expansión cuando encuentra un '$' en la cadena.
** Llama a la función correspondiente según el tipo de variable.
*/
int		handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell);

#endif
