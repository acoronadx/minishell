/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:24:17 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/04 12:30:00 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtins.h"

// ----------- PWD -----------
int	run_pwd(void)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX))
	{
		ft_putendl_fd(buf, 1);
		return (0);
	}
	ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
	return (1);
}
