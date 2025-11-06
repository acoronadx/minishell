/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:22:42 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:05:14 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "env.h"
#include "exec.h"
#include "minishell.h"
#include <errno.h>
#include <sys/stat.h>

int	is_executable(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	if (S_ISDIR(st.st_mode))
	{
		errno = EISDIR;
		return (0);
	}
	return (access(path, X_OK) == 0);
}

char	*join_path(const char *dir, const char *file)
{
	char	*tmp;
	size_t	len_dir;
	size_t	len_file;
	size_t	i;

	len_dir = ft_strlen(dir);
	len_file = ft_strlen(file);
	tmp = malloc(len_dir + 1 + len_file + 1);
	if (!tmp)
		return (NULL);
	ft_memcpy(tmp, dir, len_dir);
	tmp[len_dir] = '/';
	i = 0;
	while (i < len_file)
	{
		tmp[len_dir + 1 + i] = file[i];
		i++;
	}
	tmp[len_dir + 1 + len_file] = '\0';
	return (tmp);
}

static int	has_slash(char *cmd)
{
	int	i;

	if (!cmd)
		return (0);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*find_path_executable(char *cmd, char **paths)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (is_executable(full_path))
			return (full_path); // No liberar, lo devolvemos
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*res;
	char	*path_value;

	if (!cmd)
		return (NULL);
	if (has_slash(cmd))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_value = get_env_value(shell, "PATH");
	if (!path_value || path_value[0] == '\0')
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	res = find_path_executable(cmd, paths);
	ft_free_strtab(paths);
	return (res);
}
