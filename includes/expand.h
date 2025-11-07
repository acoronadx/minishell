/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:17:25 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/07 22:46:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"
# include "env.h"
# include "lexer.h"
# include "parser.h"

/* Interfaz principal de expansión de tokens */
char   *expand_token(const char *str, t_shell *shell);
int     expand_variables(t_shell *shell);

/* Funciones internas */
size_t  calculate_expanded_len(const char *str, t_shell *shell);
char   *expand_value(const char *str, int *i, t_shell *shell);
char   *expand_tilde_internal(const char *str, t_shell *shell);
size_t  get_tilde_prefix_len(const char *str);
void    perform_word_splitting(t_shell *shell);
void    remove_quotes(t_token *tokens);
char	*expand_heredoc_line(t_shell *sh, const char *line);
t_token	*split_word_respecting_quotes(const char *s);  /* split respetando comillas */
char	*quote_remove_like_bash(const char *s);  /* quote-removal como bash */
char	*quote_remove_for_delim(const char *s); /* quote-removal para delimitador heredoc */

/* Utils que ya tienes */
char   *find_var(t_env *env, char *name);

#endif
