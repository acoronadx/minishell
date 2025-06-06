/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 21:50:59 by acoronad          #+#    #+#             */
/*   Updated: 2025/05/30 12:55:44 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include <stddef.h>
# include <unistd.h>
# include <stdarg.h>
# include "../libft.h"

// Funciones principales
void	ft_dprintf(int fd, const char *format, ...);
void	ft_dprintftype(char **bf, char type, va_list put);

// Funciones auxiliares para impresión
char	*ft_joinandfree(char *s1, char s2);
char	*ft_joinandfreestr(char *s1, const char *s2);
void	ft_pint_bs_bf(char **bf, unsigned int nbr, const char *base);
void	ft_putptr_bf(char **bf, void *ptr);
void	ft_putunsignedint_bf(char **bf, unsigned int n);
void	ft_dputchar_bf(char **bf, char c);
void	ft_dputstr_bf(char **bf, const char *s);
void	ft_putnbr_bf(char **bf, int n);

#endif