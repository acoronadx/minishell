/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:37:31 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/27 19:42:31 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:22 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 06:49:17 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

typedef struct s_env t_env;

// --- Prototipos de funciones de init_env_list.c ---
t_env   *init_env_list(char **envp);
t_env	*env_create(char *key, char *value, int exported);
void	env_add_back(t_env **env, t_env *new);

// --- Prototipos de funciones de env_free.c ---
void    free_env_list(t_env *env);

#endif
