/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:52:34 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/26 14:33:25 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

char *find_var(t_env *env, char *name)
{
    while (env)
    {
        if (ft_strcmp(env->key, name) == 0)
            return (env->value ? env->value : "");
        env = env->next;
    }
    return "";
}

char *get_program_name_str(t_shell *shell)
{
    if (!shell->program_name)
        return ft_strdup("minishell");
    return ft_strdup(shell->program_name);
}
