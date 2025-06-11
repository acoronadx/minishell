/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:10:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/10 13:54:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"


void	print_usage(const char *invalid);
void	setup_signals(void);
void	run_interactive(char **envp);
void	execute_one_command(const char *command, char **envp);
void	execute_script(const char *script_file, char **envp);
int		ft_quotes_closed(const char *line);
void	handle_sigint(int sig);
