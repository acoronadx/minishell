# **************************************************************************** #
#                                MINISHELL MAKEFILE                            #
# **************************************************************************** #

NAME        = minishell

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g
INCLUDES    = -Iincludes -Ilibft -Ilibft/ft_dprintf -Ilibft/gnl

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
DPRINTF     = $(LIBFT_DIR)/ft_dprintf/libftdprintf.a

SRC_DIR     = src
OBJ_DIR     = obj

# Encuentra todos los .c en src y añade minishell.c
SRC         = $(shell find $(SRC_DIR) -name "*.c") minishell.c
OBJ         = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Colores (opcional)
GREEN       := \033[0;32m
RESET       := \033[0m

# Regla para compilar objetos y crear directorios si no existen
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regla principal: compilar y enlazar ejecutable
$(NAME): $(DPRINTF) $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(DPRINTF) $(LIBFT) -o $(NAME) -lreadline
	@echo "$(GREEN)¡Minishell compilada y lista!$(RESET)"

# Compilar libft y dprintf
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(DPRINTF):
	$(MAKE) -C $(LIBFT_DIR)/ft_dprintf

# Limpieza de objetos y librerías
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(LIBFT_DIR)/ft_dprintf clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR)/ft_dprintf fclean

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re