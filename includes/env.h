/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/11 15:27:24 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

t_env	*init_env_list(char **envp);
void	free_env_list(t_env *env);
t_env	*get_env_node(t_env *env, const char *key);
char	*get_env_value_any(t_env *env, const char *key);
char	*get_env_value(t_env *env, const char *key);
int		add_env_var(t_env **env, const char *key, const char *value, int exported);
int		update_env_var(t_env *env, const char *key, const char *value, int exported);
int		remove_env_var(t_env **env, const char *key);
char	**env_list_to_envp(t_env *env);

#endif