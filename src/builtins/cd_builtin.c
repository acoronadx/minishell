/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:25:12 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/01 15:15:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"
#include "env.h"

// ----------- CD -----------
int	run_cd(char **argv, t_shell *shell)
{
	const char	*path;
	char		*home;

	/* demasiados args */
	if (argv[1] && argv[2])
	{
		ft_dprintf(2, "minishell: cd: too many arguments\n");
		return (1);
	}

	path = argv[1];
	if (!path)
	{
		home = get_env_value(shell, "HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		path = home;
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	return (0);
}
