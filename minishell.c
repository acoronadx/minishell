/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:46:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/25 17:17:06 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "signals.h"
//#include "exec.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		ret;

//	setup_prompt_signals();
	setup_signals();
	if (init_shell_name(&shell, argv) != 0)
		return (1);
	ret = run_shell_modes(argc, argv, envp, &shell);
	cleanup_shell(&shell);
	return (ret);
}
