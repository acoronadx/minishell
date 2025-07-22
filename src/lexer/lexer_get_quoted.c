/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_quoted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:54:51 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/22 12:35:14 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static t_quote    get_quote_type(char c)
{
   if (c == '\'')
       return (SINGLE_QUOTE);
   else if (c == '"')
       return (DOUBLE_QUOTE);
   return (NO_QUOTE); // Esto no debería ocurrir si se llama correctamente
}

/* Helper para extraer la subcadena entre comillas y tipo de quote */
static int get_quoted_str(const char *line, int i, char **quoted_str, t_quote *qtype)
{
   char    quote_char;
   int     start;
   int     end;

   quote_char = line[i]; // El carácter de la comilla inicial
   *qtype = get_quote_type(quote_char);
   start = i; // El inicio de la subcadena incluye la comilla de apertura
   end = i + 1; // Empezamos a buscar la comilla de cierre después de la de apertura

   while (line[end] && line[end] != quote_char)
       end++;
   
   if (!line[end]) // Si llegamos al final de la línea y no encontramos la comilla de cierre
       return (-1); // Indica una comilla no cerrada

   end++; // El final de la subcadena incluye la comilla de cierre

   *quoted_str = ft_substr(line, start, end - start);
   if (!*quoted_str)
       return (-1);
   return (end); // Devolvemos el índice después de la comilla de cierre
}

int  get_quoted(const char *line, int i, t_token **lst)
{
   char    *quoted_str;
   t_quote qtype;
   int     next_i;

   next_i = get_quoted_str(line, i, &quoted_str, &qtype);
   if (next_i == -1) // Error al obtener la cadena entrecomillada
   {
       free_lexer_list_on_error(lst);
       return (-1);
   }
   // Añadir el token. El 'value' contendrá las comillas (ej. "'hola'"), y 'qtype' indicará si era SINGLE o DOUBLE.
   if (try_add_token(lst, quoted_str, T_WORD, qtype) == 0)
   {
       free(quoted_str); // Si try_add_token falla, liberamos la cadena
       return (-1); // Indicar error
   }
   return (next_i); // Devolver el índice donde termina este token
}