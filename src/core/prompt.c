/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 20:28:09 by codespace         #+#    #+#             */
/*   Updated: 2025/06/06 20:29:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_prompt(void)
{
	char	cwd[PATH_MAX];
	char	*user;
	char	*home;

	user = getenv("USER");
	if (!user)
		user = "minishell";
	if (!getcwd(cwd, sizeof(cwd)))
	{
		cwd[0] = '?';
		cwd[1] = '\0';
	}
	home = getenv("HOME");
	if (home && ft_strnstr(cwd, home, ft_strlen(home)) == cwd)
		printf("%s:~%s$ ", user, cwd + ft_strlen(home));
	else
		printf("%s:%s$ ", user, cwd);
}
