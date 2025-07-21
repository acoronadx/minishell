/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:49:35 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/21 14:18:58 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "minishell.h"

static char *strip_quotes_from_value(const char *value)
{
    char    *new_value;
    int     i; // Index for original value
    int     j; // Index for new_value
    t_quote current_quote_state; // Estado de las comillas que estamos procesando (NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE)

    if (!value)
        return (NULL);

    new_value = ft_calloc(ft_strlen(value) + 1, sizeof(char));
    if (!new_value)
        return (NULL);

    i = 0;
    j = 0;
    current_quote_state = NO_QUOTE;

    while (value[i])
    {
        if (value[i] == '\'')
        {
            if (current_quote_state == NO_QUOTE) // Entra en comillas simples (delimitadoras)
            {
                current_quote_state = SINGLE_QUOTE;
                // No copiamos la comilla, se elimina (es delimitadora)
            }
            else if (current_quote_state == SINGLE_QUOTE) // Sale de comillas simples (delimitadoras)
            {
                current_quote_state = NO_QUOTE;
                // No copiamos la comilla, se elimina
            }
            else if (current_quote_state == DOUBLE_QUOTE) // Dentro de comillas dobles, ' es literal
            {
                new_value[j++] = value[i]; // Copia la comilla simple literal
            }
            i++;
        }
        else if (value[i] == '"')
        {
            if (current_quote_state == NO_QUOTE) // Entra en comillas dobles (delimitadoras)
            {
                current_quote_state = DOUBLE_QUOTE;
                // No copiamos la comilla, se elimina
            }
            else if (current_quote_state == DOUBLE_QUOTE) // Sale de comillas dobles (delimitadoras)
            {
                current_quote_state = NO_QUOTE;
                // No copiamos la comilla, se elimina
            }
            else if (current_quote_state == SINGLE_QUOTE) // Dentro de comillas simples, " es literal
            {
                new_value[j++] = value[i]; // Copia la comilla doble literal
            }
            i++;
        }
        else if (value[i] == '\\' && value[i+1]) // Manejar backslashes
        {
            // La lógica de backslash debe ser la misma que en expand_token
            // para asegurar que las reglas de Bash se aplican consistentemente.
            // Si expand_token ya eliminó el backslash al copiar, aquí no debería haber uno.
            // Si expand_token lo dejó (ej. `\` en `'hola\'mundo'`), aquí debemos manejarlo.
            
            // Revisa qué backslashes `expand_token` conserva.
            // Generalmente, Bash elimina el backslash si escapa un carácter.
            // Los casos son:
            // 1. Fuera de comillas: `\` + cualquier_caracter. `\` se quita.
            // 2. Dentro de `"`: `\` + `$` `\` + `"` `\` + `\`. `\` se quita.
            // 3. Dentro de `'`: `\` es literal.
            
            // Si el backslash estaba para escapar algo y ya lo manejó expand_token,
            // no lo encontraremos aquí. Si lo encontramos, es literal.
            
            // Por simplicidad en quote_removal: si lo vemos aquí y no estamos en SINGLE_QUOTE,
            // y el siguiente char es algo que Bash escaparía y quitaría el backslash, lo quitamos.
            if (current_quote_state != SINGLE_QUOTE && 
                ((current_quote_state == DOUBLE_QUOTE && (value[i+1] == '$' || value[i+1] == '"' || value[i+1] == '\\')) ||
                 current_quote_state == NO_QUOTE)) // Si NO_QUOTE, \ escapa cualquier cosa
            {
                new_value[j++] = value[i+1]; // Copia el caracter escapado
                i += 2;
            }
            else // Es un backslash literal (ej. dentro de comillas simples o no escapa)
            {
                new_value[j++] = value[i++];
            }
        }
        else // Caracter normal
        {
            new_value[j++] = value[i++];
        }
    }
    new_value[j] = '\0';
    return (new_value);
}

void    remove_quotes(t_token *tokens)
{
    t_token *current_token;
    char    *stripped_value;

    current_token = tokens;
    while (current_token)
    {
        // En este enfoque, strip_quotes_from_value debe procesar todas las comillas
        // y el campo current_token->quoted ya no se usa para decidir la eliminación.
        // Se llama strip_quotes_from_value sin pasar el tipo de quote,
        // ya que la propia función interna debe rastrear el estado de las quotes.
        stripped_value = strip_quotes_from_value(current_token->value);
        if (!stripped_value)
        {
            // Manejo de error
            return ;
        }
        free(current_token->value);
        current_token->value = stripped_value;
        // El token ya no tiene comillas de contención relevantes para la ejecución.
        current_token->quoted = NO_QUOTE; 
        current_token = current_token->next;
    }
}
