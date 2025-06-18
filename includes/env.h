/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/17 19:13:11 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ENV_H
# define ENV_H

# include "minishell.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

// env_array.c
int		env_count_exported(t_env *env);
char	**env_list_to_envp(t_env *env);

// env_free.c
void	free_env_list(t_env *env);

// env_get.c
t_env	*get_env_node(t_env *env, char *key);
char	*get_env_value_any(t_env *env, char *key);
char	*get_env_value(t_env *env, char *key);

// env_modify.c
int		add_env_var(t_env **env, char *key, char *value, int exported);
int		update_env_var(t_env *env, char *key, char *value, int exported);
int		remove_env_var(t_env **env, char *key);
t_env	*env_create(char *key, char *value, int exported);
void	env_add_back(t_env **env, t_env *new);

// init_env_list.c
t_env	*init_env_list(char **envp);

// find_in_path.c
char	*join_path(char *dir, char *cmd);
char	*find_in_path(char *cmd, t_env *env);
char	*find_in_dirs(char **dirs, char *cmd);

#endif
