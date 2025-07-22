/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:25:33 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 12:35:57 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

// word_end: Esta función debe encontrar el final de una "palabra"
// Una palabra termina cuando encuentra un espacio (no escapado), un operador (no escapado)
// o el final de la línea. Las comillas internas a la palabra (ej. 'foo"bar"baz')
// NO deben detener el word_end.
static int    word_end(const char *line, int i)
{
   int escaped = 0; // Para manejar backslashes que escapan el siguiente caracter

   while (line[i])
   {
       // Manejo de backslashes: si encontramos '\' y no estamos ya escapados
       if (line[i] == '\\' && !escaped)
       {
           escaped = 1; // Marcamos que el siguiente caracter está escapado
           i++;
           continue ;
       }
       // Si encontramos un espacio O un operador Y NO estamos escapados, la palabra termina
       if ((ft_isspace(line[i]) || is_operator(line + i, NULL, NULL)) && !escaped)
           break ;
       
       // Importante: Las comillas (' o ") DENTRO de una palabra (ej. "foo'bar'")
       // NO deben romper la palabra en el léxer. Se capturan como parte del valor.
       // Su efecto se manejará en la fase de expansión/quote removal.
       
       escaped = 0; // Reseteamos el estado de escape para el siguiente caracter
       i++;
   }
   return (i);
}

// remove_backslashes: Esta función es útil para eliminar backslashes
// que escapan caracteres fuera de comillas, o en contextos específicos.
// Su implementación actual (la que tenías antes del diff) debería ser correcta aquí.
// La pego para referencia, pero asumo que ya la tienes bien.
char    *remove_backslashes(const char *src)
{
   char    *dst;
   int     i;
   int     j;
   int     escaped;

   i = 0;
   j = 0;
   escaped = 0;
   // Calcular la longitud final sin los backslashes que se eliminarán
   int len = 0;
   int k = 0;
   while (src[k])
   {
       if (src[k] == '\\' && !escaped)
       {
           escaped = 1;
           k++;
           continue ;
       }
       len++;
       escaped = 0;
       k++;
   }

   dst = malloc(sizeof(char) * (len + 1));
   if (!dst)
       return (NULL);
   
   escaped = 0;
   while (src[i])
   {
       if (src[i] == '\\' && !escaped)
       {
           escaped = 1;
           i++;
           continue ;
       }
       dst[j++] = src[i++];
       escaped = 0;
   }
   dst[j] = '\0';
   return (dst);
}

int    get_word(const char *line, int i, t_token **lst)
{
   int     start;
   int     len;
   char    *word_str;
   char    *processed_str; // Cambiado de 'unescaped' para mayor claridad

   start = i;
   i = word_end(line, i); // Encontrar el final de la palabra
   len = i - start;
   if (len == 0) // No debería pasar si word_end funciona correctamente, pero es una buena salvaguarda
       return (i);
   
   word_str = ft_substr(line, start, len); // Extraer la subcadena
   if (!word_str)
       return (free_lexer_list_on_error(lst), -1);

   // Aquí podemos aplicar `remove_backslashes`.
   // Nota: Los backslashes dentro de comillas (ej. "foo\"bar") formarán parte de `word_str`
   // y esta función los procesará. Esto suele ser aceptable, ya que las reglas finales de
   // quote removal se encargarán de ellos.
   processed_str = remove_backslashes(word_str);
   free(word_str); // Liberar la cadena original
   if (!processed_str)
       return (free_lexer_list_on_error(lst), -1);

   // Añadir el token a la lista. El 'value' contendrá la palabra sin backslashes (si aplica)
   // y el tipo de comilla será NO_QUOTE para las palabras obtenidas por get_word.
   if (try_add_token(lst, processed_str, T_WORD, NO_QUOTE) == 0)
   {
       free(processed_str); // Si try_add_token falla, liberar la cadena
       return (-1);
   }
   return (i); // Devolver el índice donde termina este token
}
