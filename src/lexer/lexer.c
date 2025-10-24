/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:07:05 by acoronad          #+#    #+#             */
/*   Updated: 2025/07/27 00:43:46 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "lexer.h"

static t_token *free_null_token_list(t_token **lst)
{
    free_token_list(*lst);
    *lst = NULL;
    return (NULL);
}

// Helper: detecta si hay un "1>", "1>>", "1<<", "0<", "2>&1", ">&1", "<&0", etc. 
// y devuelve el desplazamiento para que el lexer trate todos los casos equivalentes.

static int  is_default_fd_redir(const char *line)
{
    if (line[0] == '1')
    {
        if (line[1] == '>' && line[2] == '>')
            return (1); // 1>> igual que >>
        if (line[1] == '>')
            return (1); // 1> igual que >
        if (line[1] == '<' && line[2] == '<')
            return (1); // 1<< igual que <<
    }
    if (line[0] == '0')
    {
        if (line[1] == '<' && line[2] == '<')
            return (1); // 0<< igual que <<
        if (line[1] == '<')
            return (1); // 0< igual que <
    }
    if (line[0] == '2')
    {
        if (line[1] == '>' && line[2] == '&' && line[3] == '1')
            return (1); // 2>&1 igual que >&1
    }
    return (0);
}

// Helper: detecta si tras el operador hay un fd por defecto y lo ignora (ej: >1, >>1, <<1, >&1, <&0)
static int  is_default_fd_suffix(const char *line)
{
    if (line[0] == '>' && line[1] == '1' && line[2] != '>')
        return (1); // >1 igual que >
    if (line[0] == '>' && line[1] == '>' && line[2] == '1')
        return (1); // >>1 igual que >>
    if (line[0] == '<' && line[1] == '<' && line[2] == '1')
        return (1); // <<1 igual que <<
    if (line[0] == '>' && line[1] == '&' && line[2] == '1')
        return (1); // >&1 igual que >&
    if (line[0] == '<' && line[1] == '&' && line[2] == '0')
        return (1); // <&0 igual que <&
    if (line[0] == '<' && line[1] == '<' && line[2] == '<' && line[3] == '1')
        return (1); // <<<1 igual que <<<
    return (0);
}

t_token    *lexer(const char *line)
{
   int     i;
   int     next_i;
   t_token *lst;
   int     skip_fd;

   i = 0;
   lst = NULL;
   while (line[i])
   {
       // Saltar espacios iniciales
       while (line[i] && ft_isspace(line[i]))
           i++;
       if (!line[i]) // Si solo quedan espacios, salimos
           break ;
       
       next_i = i;
       skip_fd = is_default_fd_redir(line + i); // Manejar "2>" etc.
       if (skip_fd)
           i += skip_fd; // Avanzar 'i' más allá del número (ej. '2')

       if (is_operator(line + i, NULL, NULL)) // Comprobar si es un operador ('>', '<<', '&&', etc.)
       {
           next_i = get_operator(line, i, &lst);
           if (is_default_fd_suffix(line + next_i)) // Manejar ">&1" o "<&-"
               next_i += 1;
       }
       // ELIMINAMOS EL else if (line[i] == '\'' || line[i] == '"') Y LA LLAMADA A get_quoted
       // Ahora, TODAS las palabras (incluyendo las que empiezan con comillas)
       // serán manejadas por get_word.
       else 
           next_i = get_word(line, i, &lst); // Llamar a get_word
       
       if (next_i == -1) // Si hubo un error en la tokenización
           return (free_null_token_list(&lst));
       i = next_i; // Mover al inicio del siguiente token
   }
   return (lst);
}
