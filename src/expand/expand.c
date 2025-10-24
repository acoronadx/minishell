/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:45:20 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 15:18:33 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "expand.h" // Asegúrate de que esta cabecera tenga las declaraciones necesarias (t_quote, etc.)
#include "env.h"    // Para t_shell y manejo de entorno

// Función auxiliar: determina si un carácter es escapable con '\' dentro de comillas dobles.
// Bash escapa: $ ` " \ newline
static bool is_double_quote_escaped_char_exp(char c)
{
    return (c == '$' || c == '`' || c == '"' || c == '\\' || c == '\n');
}

// expand_token: Realiza la expansión de variables ($VAR) y tilde (~).
// También maneja la eliminación de ciertos backslashes.
// NO elimina las comillas delimitadoras finales, eso es trabajo de strip_quotes_from_value.
char *expand_token(const char *str, t_shell *shell)
{
    char    *res;
    int     i; // Índice para recorrer str (input)
    int     j; // Índice para construir res (output)
    t_quote current_quote_state; // NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE
    int     added_len;

    if (str == NULL)
        return (NULL);

    // Asignación de memoria generosa para evitar reallocs.
    // Un tamaño `ft_strlen(str) * 2 + 1` es una estimación razonable para la expansión.
    res = ft_calloc(ft_strlen(str) * 2 + 1, sizeof(char));
    if (!res)
        return (NULL);

    i = 0;
    j = 0;
    current_quote_state = NO_QUOTE;

    while (str[i])
    {
        // 1. Manejo de Comillas Delimitadoras
        // Las comillas se copian literalmente aquí, su eliminación final es para strip_quotes_from_value.
        if (str[i] == '\'')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = SINGLE_QUOTE;
            else if (current_quote_state == SINGLE_QUOTE)
                current_quote_state = NO_QUOTE;
            res[j++] = str[i++];
            continue;
        }
        else if (str[i] == '"')
        {
            if (current_quote_state == NO_QUOTE)
                current_quote_state = DOUBLE_QUOTE;
            else if (current_quote_state == DOUBLE_QUOTE)
                current_quote_state = NO_QUOTE;
            res[j++] = str[i++];
            continue;
        }

        // 2. Manejo de Backslashes (¡LA CLAVE ESTÁ AQUÍ!)
        if (str[i] == '\\')
        {
            if (current_quote_state == SINGLE_QUOTE) // Dentro de comillas simples: '\' es SIEMPRE literal
            {
                res[j++] = str[i++]; // Copia '\' tal cual. strip_quotes_from_value la mantendrá o quitará.
                continue;
            }
            else if (current_quote_state == DOUBLE_QUOTE) // Dentro de comillas dobles
            {
                // Bash escapa: $ ` " \ newline. Si escapa uno de estos, el '\' se elimina.
                if (str[i + 1] && is_double_quote_escaped_char_exp(str[i + 1]))
                {
                    i++; // Saltar la barra invertida (la eliminamos)
                    res[j++] = str[i++]; // Copiar el carácter escapado
                    continue;
                }
                // Si '\' escapa cualquier otra cosa (ej. "\a" en comillas dobles),
                // o si '\' es el último caracter, '\' es literal y NO se elimina aquí.
                else
                {
                    res[j++] = str[i++]; // Copia '\' tal cual. strip_quotes_from_value la mantendrá.
                    continue;
                }
            }
            else // current_quote_state == NO_QUOTE (fuera de comillas)
            {
                // ¡CORRECCIÓN CRÍTICA!
                // En Bash, fuera de comillas, '\' SOLO se elimina si escapa un salto de línea.
                // Para CUALQUIER otro carácter (incluyendo '$', comillas como '\'' o '\"', 'a'),
                // el '\' se COPIA LITERALMENTE en expand_token.
                // Su eliminación (o no) se delega a `strip_quotes_from_value`.
                // Esto explica el comportamiento de `\$USER` (elimina `\`) vs `\'simple` (mantiene `\`).
                // Bash elimina `\` para `$` en expansión y `\` para comillas en quote removal.
                // Para simplificar y seguir el comportamiento de Bash:

                if (str[i + 1] == '\n') // Si '\' escapa un salto de línea, se elimina
                {
                    i++; // Saltar el backslash
                    res[j++] = str[i++]; // Copiar el newline
                    continue;
                }
                else if (str[i + 1] == '$') // Si '\' escapa un '$', se elimina para que el '$' sea literal
                {
                    i++; // Saltar el backslash
                    res[j++] = str[i++]; // Copiar el '$'
                    continue;
                }
                else // Para cualquier otro caracter (incluyendo comillas o letras), '\' es literal
                {
                    res[j++] = str[i++]; // Copiar el '\' tal cual
                    if (str[i]) // Si hay un caracter después del '\'
                    {
                        res[j++] = str[i++]; // Copiar el caracter escapado (ej. 'a' de '\a')
                    }
                    continue;
                }
            }
        }

        // 3. Expansión de $ (solo si no estamos dentro de comillas simples y no ha sido escapado por '\' justo antes)
        // Nota: Si el `\$` se manejó arriba (eliminando `\`), `str[i]` ahora será `$`, pero ya se copió,
        // así que no debería volver a entrar aquí para expandirse.
        if (current_quote_state != SINGLE_QUOTE && str[i] == '$')
        {
            // Verifica que el '$' no fue procesado justo antes por el manejo de `\`
            // (esto es implícito si el `continue` de `\` se usa correctamente)
            added_len = handle_dollar(str, &i, res, j, shell);
            if (added_len == -1)
            {
                free(res);
                return (NULL);
            }
            j += added_len;
            continue;
        }
        // 4. Manejo de Tilde (tu lógica existente debería seguir funcionando)
        else if (str[i] == '~' && (i == 0 || ft_isspace(str[i - 1]) || str[i - 1] == '='))
        {
            char *tilde_expanded = expand_tilde_internal(str + i, shell);
            if (!tilde_expanded)
            {
                free(res);
                return (NULL);
            }
            ft_strcpy(res + j, tilde_expanded);
            j += ft_strlen(tilde_expanded);
            i += get_tilde_prefix_len(str + i);
            free(tilde_expanded);
            continue;
        }
        
        // 5. Copiar cualquier otro carácter normal
        res[j++] = str[i++];
    }
    res[j] = '\0'; // Asegurarse de que la cadena resultante esté terminada en nulo
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
        // expand_token es responsable de la expansión de variables ($VAR)
        // Y también de manejar backticks (`` ` ``) si los implementas
        // ¡Importante!: expand_token NO debe hacer quote removal.
        // Debe devolver el valor con las comillas y backslashes "intermedios" intactos.
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
    // Esto asegura que después de la expansión, se eliminan las comillas y se procesan los backslashes.
    remove_quotes(shell->tokens);
    return (0);
}

