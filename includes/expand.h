/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:17:25 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:31:54 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"
# include "env.h"

/* expand.c */
int     expand_variables(t_shell *shell);
char    *expand_token(const char *str, t_shell *shell);

/* expand_dollar.c */
char    *expand_value(const char *str, int *i, t_shell *shell);
int     handle_dollar(const char *str, int *i, char *dst, int j, t_shell *shell);

/* calculate_expand_len.c */
size_t  calculate_expanded_len(const char *str, t_shell *shell);
size_t  handle_dollar_len(const char *str, int *i, t_shell *shell);

/* handle_expand_cases.c */
size_t  handle_pid_len(void);
size_t  handle_exit_status_len(t_shell *shell);
char    *get_program_name_str(t_shell *shell);

/* expand_tilde.c */
char    *expand_tilde_internal(const char *str, t_shell *shell);
size_t  get_tilde_prefix_len(const char *str);

/* expand_utils.c */
char    *find_var(t_env *env, char *name);

/* quote_removal.c */
void    remove_quotes(t_token *tokens);

/* word_splitting.c */
void    perform_word_splitting(t_shell *shell);

#endif
