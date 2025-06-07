# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codespace <codespace@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/16 15:09:12 by rarodri2          #+#    #+#              #
#    Updated: 2025/06/07 01:13:25 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes

DPRINTF_DIR = ft_dprintf
DPRINTF_LIB = $(DPRINTF_DIR)/libftdprintf.a

SRCS = ft_atoi.c ft_atol.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
	ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_isquote.c ft_issymbol.c ft_itoa.c ft_memchr.c \
	ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
	ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
	ft_strdup.c ft_strldup.c ft_striteri.c ft_strjoin.c ft_strjoin_free.c ft_strlcat.c ft_strlcpy.c \
	ft_strlen.c ft_arraylen.c ft_strmapi.c ft_strcmp.c ft_strcpy.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
	ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_printf.c ft_putchar.c \
	ft_puthex.c ft_putnbr.c ft_putptr.c ft_putstr.c ft_putunsigned.c get_next_line.c \
	ft_strcat.c 
OBJ = $(SRCS:.c=.o)
INCLUDE = libft.h
BONUSSRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
	ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c
BONUSOBJ = $(BONUSSRC:.c=.o)

all: $(DPRINTF_LIB) $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)
	@echo "\033[32m✔ libft.a compilada con éxito\033[0m"

$(DPRINTF_LIB):
	$(MAKE) -C $(DPRINTF_DIR)

bonus: $(OBJ) $(BONUSOBJ) $(DPRINTF_LIB)
	ar rcs $(NAME) $(OBJ) $(BONUSOBJ)
	@echo "\033[32m✔ libft.a (bonus) compilada con éxito\033[0m"

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUSOBJ)
	$(MAKE) -C $(DPRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(DPRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus