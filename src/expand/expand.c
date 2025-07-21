/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/21 14:10:19 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h"
#include "env.h"

// Eliminamos skip_empty_quotes porque su lógica será parte del manejo de comillas general

// Función principal para expandir y reconstruir el token
char    *expand_token(const char *str, t_shell *shell)
{
    char    *res;
    int     i;          // Índice para recorrer str (input)
    int     j;          // Índice para construir res (output)
    t_quote current_quote_state; // NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE
    int     added_len;
    
    // Calcula la longitud necesaria (la modificaremos después)
    size_t required_len = calculate_expanded_len(str, shell);
    if (required_len == (size_t)-1)
        return (NULL);
    
    res = ft_calloc(required_len + 1, sizeof(char));
    if (!res)
        return (NULL);

    i = 0;
    j = 0;
    current_quote_state = NO_QUOTE; // Inicialmente fuera de comillas

    while (str[i])
    {
        // 1. Manejo de Comillas
        if (str[i] == '\'')
        {
            if (current_quote_state == NO_QUOTE) // Entra en comillas simples
                current_quote_state = SINGLE_QUOTE;
            else if (current_quote_state == SINGLE_QUOTE) // Sale de comillas simples
                current_quote_state = NO_QUOTE;
            // Copia la comilla al resultado. El quote removal final la quitará.
            res[j++] = str[i++];
            continue; // Pasa al siguiente caracter
        }
        else if (str[i] == '"')
        {
            if (current_quote_state == NO_QUOTE) // Entra en comillas dobles
                current_quote_state = DOUBLE_QUOTE;
            else if (current_quote_state == DOUBLE_QUOTE) // Sale de comillas dobles
                current_quote_state = NO_QUOTE;
            // Copia la comilla al resultado. El quote removal final la quitará.
            res[j++] = str[i++];
            continue; // Pasa al siguiente caracter
        }

        // 2. Expansión (solo si no estamos dentro de comillas simples)
        if (current_quote_state != SINGLE_QUOTE)
        {
            // Manejo de Backslash antes de $ (Bash solo escapa $ dentro de comillas dobles)
            if (str[i] == '\\' && str[i + 1] && current_quote_state == DOUBLE_QUOTE)
            {
                // Dentro de comillas dobles, el backslash solo escapa $, `, ", \, newline.
                // Aquí simplificamos a solo escapar $
                if (str[i + 1] == '$' || str[i + 1] == '"' || str[i + 1] == '\\')
                {
                    res[j++] = str[i + 1]; // Copia el caracter escapado
                    i += 2; // Salta \ y el caracter escapado
                    continue;
                }
            }
            // Manejo de $
            if (str[i] == '$')
            {
                added_len = handle_dollar(str, &i, res, j, shell); // handle_dollar avanzará 'i'
                if (added_len == -1) // Error de memoria
                {
                    free(res);
                    return (NULL);
                }
                j += added_len;
                continue; // Pasa al siguiente caracter después de la expansión
            }
            // Manejo de Tilde
            else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1]) || str[i - 1] == '=')) // Expandir ~ solo al inicio de palabra o después de '='
            {
                char *tilde_expanded = expand_tilde_internal(str + i, shell);
                if (!tilde_expanded)
                {
                    free(res);
                    return (NULL);
                }
                ft_strcpy(res + j, tilde_expanded);
                j += ft_strlen(tilde_expanded);
                i += get_tilde_prefix_len(str + i); // Asegúrate que esto avanza 'i' correctamente
                free(tilde_expanded);
                continue;
            }
        }
        
        // 3. Copiar caracteres normales o escapar '\' fuera de comillas dobles (Bash)
        // Fuera de comillas dobles, '\' escapa cualquier caracter. Bash quita el '\'.
        if (str[i] == '\\' && str[i + 1] && current_quote_state == NO_QUOTE)
        {
            res[j++] = str[i + 1]; // Copia el caracter escapado, el backslash se elimina
            i += 2;
            continue;
        }

        // Si no es una comilla, $, ~, o backslash especial, copia el caracter tal cual
        res[j++] = str[i++];
    }
    res[j] = '\0';
    return (res);
}

// expand_variables queda igual, pero ahora remove_quotes se llama al final de expand_variables
int expand_variables(t_shell *shell)
{
    t_token *current_token;
    char    *expanded_value;

    if (!shell->tokens)
        return (0);
    current_token = shell->tokens;
    while (current_token)
    {
        // En este enfoque, no usamos current_token->quoted para la decisión de expansión
        // interna, ya que expand_token manejará las comillas dentro del value.
        // Si tu lexer ya clasificó el token entero como SINGLE_QUOTE,
        // podrías seguir usando: if (current_token->quoted != SINGLE_QUOTE)
        // para un rendimiento óptimo, ya que expand_token no necesitaría
        // analizar las comillas internas para ese token.
        // Pero si el lexer pasa `'foo'bar` como NO_QUOTE, expand_token debe manejarlo.
        
        // Asumiendo que el lexer *no* está clasificando 'quoted' para las palabras internas
        // o que lo clasifica como NO_QUOTE/OTHER, expand_token debe hacer todo el trabajo.
        expanded_value = expand_token(current_token->value, shell);
        if (!expanded_value)
        {
            free_token_list(shell->tokens);
            shell->tokens = NULL;
            return (-1);
        }
        free(current_token->value);
        current_token->value = expanded_value;
        current_token = current_token->next;
    }
    // La llamada a remove_quotes sigue aquí, al final de la fase de expansión
    remove_quotes(shell->tokens);
    return (0);
}
