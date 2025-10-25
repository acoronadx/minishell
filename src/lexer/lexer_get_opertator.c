/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_opertator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:55:24 by acoronad          #+#    #+#             */
/*   Updated: 2025/10/24 14:41:35 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

int     get_operator(const char *line, int i, t_token **lst)
{
        t_token_type    type;
        int             len;
        char            *op_str;

        if (!is_operator(line + i, &type, &len))
                return (i);
        op_str = ft_substr(line, i, len);
        if (!op_str)
        {
                free_lexer_list_on_error(lst);
                return (-1);
        }
        if (!try_add_token(lst, op_str, type, NO_QUOTE))
                return (-1);
        return (i + len);
}
