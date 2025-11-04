/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:37:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/11/04 14:19:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stddef.h>

typedef struct s_shell t_shell;

/* Definición propia de entorno */
typedef struct s_env
{
	char            *key;
	char            *value;
	struct s_env    *next;
	int             exported;
}   t_env;

/* init_env_list.c */
t_env	*init_env_list(char **envp);
t_env	*env_create(char *key, char *value, int exported);
void	env_add_back(t_env **env, t_env *new);

/* free_env_list.c */
void	free_env_list(t_env *env);

/* get_env_value.c | get_env_array.c */
char	*get_env_value(t_shell *shell, const char *key);
char	**env_to_array(t_env *env);

#endif
