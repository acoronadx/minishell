/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoronad <acoronad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:07:19 by acoronad          #+#    #+#             */
/*   Updated: 2025/06/13 19:46:15 by acoronad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

// Prototipos principales del parser
t_ast	*parse_command_line(t_token **cur);
t_ast	*parse_background(t_token **cur);
t_ast	*parse_sequence(t_token **cur);
t_ast	*parse_and_or(t_token **cur);
t_ast	*parse_pipeline(t_token **cur);
t_ast	*parse_simple_command(t_token **cur);
void	next_token(t_token **cur);

#endif
