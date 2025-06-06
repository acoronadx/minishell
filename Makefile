# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/23 11:12:01 by rarodri2          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/06/02 01:04:11 by acoronad         ###   ########.fr        #
=======
<<<<<<< HEAD
#    Updated: 2025/05/30 18:29:21 by acoronad         ###   ########.fr        #
=======
#    Updated: 2025/05/30 12:59:22 by rarodri2         ###   ########.fr        #
>>>>>>> baff6a5c9fcc0a53feece55b2b1486e927ce27da
>>>>>>> ccd245c5efa381cd07731a4c1dee0f6dc3172dd5
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
RM      = rm -f

INC_DIR = include
LIBFT_DIR = libft

SRCS    = minishell.c \
          env/init.c \
          env/cleanup.c \
          builtins/builtin_cd.c \
          builtins/builtin_echo.c \
          builtins/builtin_env.c \
          builtins/builtin_exit.c \
          builtins/builtin_export.c \
          builtins/builtin_pwd.c \
          builtins/builtin_unset.c \
          builtins/exec_builtin.c \
          parser/env_utils.c \
		  parser/parser_utils.c \
          parser/shell_exec.c \
		  parser/parser.c \
		  parser/expand.c \
		  utils/errors.c \
		  utils/ft_split_custom.c \
		  utils/parse_line.c \
          utils/print_prompt.c \

OBJS    = $(SRCS:.c=.o)

LIBFT   = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re