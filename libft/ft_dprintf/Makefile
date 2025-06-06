NAME = libftdprintf.a

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Source and object files
SRCS = ft_dprintf.c ft_dprintftype.c ft_joinandfree.c \
		ft_putint_base_bf.c ft_putunsignedint_bf.c ft_putnbr_df.c \
	   ft_putptr_bf.c ft_dputchar_bf.c ft_dputstr_bf.c

OBJS = $(SRCS:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c ft_dprintf.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAIN_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
