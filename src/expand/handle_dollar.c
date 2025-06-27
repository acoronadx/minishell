/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:15:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/28 00:15:52 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

// handle_dollar: Handles dollar sign expansion ($, $?, $$, $VAR).
// It now calls functions that *return* the expanded string,
// then copies that string into the 'res' buffer.
// It returns the number of characters written to 'res'.
int handle_dollar(const char *str, int *i, char *res, int j, t_shell *shell)
{
    char    *expanded_value; // To hold the string returned by helper functions
    int     len_copied;

    expanded_value = NULL;
    len_copied = 0;

    // Advance 'i' past the '$'
    (*i)++;

    if (str[*i] == '?')
    {
        expanded_value = ft_itoa(shell->exit_status); // Call the new function
        (*i)++; // Advance 'i' past '?'
    }
    else if (str[*i] == '0')
    {
        expanded_value = get_program_name_str(shell); // Call the new function
        (*i)++; // Advance 'i' past '0'
    }
    else if (str[*i] == '$')
    {
        expanded_value = ft_itoa(getpid()); // Call the new function
        (*i)++; // Advance 'i' past '$'
    }
    else if (ft_isdigit(str[*i])) // e.g., $1, $2 (positional parameters, usually ignored in minishell)
    {
        // For positional parameters like $1, $2, etc., usually they are ignored or
        // replaced with an empty string in basic minishells.
        // Advance 'i' past the digit.
        (*i)++;
        expanded_value = ft_strdup(""); // Replace with empty string
    }
    else if (str[*i] == '_' || ft_isalpha(str[*i]))
    {
        // Handle regular environment variables ($VAR)
        int start_var = *i;
        while (str[*i] == '_' || ft_isalnum(str[*i]))
            (*i)++;
        char *var_name = ft_substr(str, start_var, *i - start_var);
        if (!var_name)
            return (-1); // Error

        expanded_value = find_var(shell->env, var_name); // Get value from env list
        free(var_name); // Free the allocated var_name string

        if (!expanded_value)
            expanded_value = ft_strdup(""); // If not found, expand to empty string
        else
            expanded_value = ft_strdup(expanded_value); // Duplicate the found value as find_var might return a pointer to internal data
    }
    else // Handle cases like `$` or `$` followed by invalid char
    {
        expanded_value = ft_strdup("$"); // Keep the dollar sign literally
    }

    if (!expanded_value)
        return (-1); // Handle allocation failure for expanded_value

    // Copy the expanded_value into the result buffer 'res'
    // ft_strlcat is generally safer for appending to fixed-size buffers
    // but assuming 'res' is dynamically sized or large enough, ft_strlcpy/ft_strcat can work too.
    // Assuming 'res' is the buffer being built incrementally, you need to copy to 'res + j'.
    // And update 'j' based on the length copied.
    len_copied = ft_strlen(expanded_value);
    ft_strlcpy(res + j, expanded_value, len_copied + 1); // +1 for null terminator

    free(expanded_value); // Free the temporary expanded_value string

    return (len_copied); // Return the length of the string just copied to 'res'
}
