/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 06:38:08 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/26 06:47:43 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    **ft_strdup_array(char **array)
{
    char    **new_array;
    int     i;
    int     count;

    if (!array)
        return (NULL);
    count = 0;
    while (array[count])
        count++;
    new_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!new_array)
        return (NULL);
    i = 0;
    while (i < count)
    {
        new_array[i] = ft_strdup(array[i]);
        if (!new_array[i])
        {
            ft_free_strtab(new_array);
            return (NULL);
        }
        i++;
    }
    new_array[i] = NULL;
    return (new_array);
}
