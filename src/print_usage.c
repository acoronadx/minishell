/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_usage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 13:52:48 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/10 13:53:16 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_usage(const char *invalid)
{
	if (invalid)
		ft_dprintf(2, "minishell: %s: invalid option\n", invalid);
	ft_dprintf(2, "Usage:  ./minishell [option] [script-file]\n");
	ft_dprintf(2, "Options:\n");
	ft_dprintf(2, "        -c command      execute command\n");
	ft_dprintf(2, "        script-file     execute commands from script file\n");
	ft_dprintf(2, "        --help          display this help and exit\n");
	ft_dprintf(2, "        --version       output version information and exit\n");
}
