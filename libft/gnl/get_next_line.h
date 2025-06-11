/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:30:04 by acoronad          #+#    #+#             */
/*   Updated: 2024/10/18 21:24:52 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

// Define BUFFER_SIZE only if not already defined
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

// -------- Prototype of the get_next_line() function --------
// Returns the position of a character in a string.
size_t	ft_position(char *s, char c);

// Reads the next line from the file descriptor and returns it.
char	*get_next_line(int fd);

// -------- Utility function prototypes (from get_next_line_utils.c) --------
// Returns the length of a string.
size_t	ft_strlen(const char *s);

// Joins two strings into a new dynamically allocated string.
char	*ft_strjoin(char const *s1, char const *s2);

// Duplicates a string, returning a new dynamically allocated string.
char	*ft_strldup(const char *s, size_t size);

#endif
