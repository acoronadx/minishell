NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror
RM          := rm -f
MKDIR_P     := mkdir -p

# Carpetas
SRC_DIR     := src
OBJ_DIR     := obj
INC_DIR     := include
LIBFT_DIR   := libft

# Busca todos los .c en src/ y subcarpetas
SRCS        := $(shell find $(SRC_DIR) -name '*.c')
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT       := $(LIBFT_DIR)/libft.a

# Para que se creen los directorios de objetos automáticamente
DIRS        := $(sort $(dir $(OBJS)))

# Reglas
all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

# Regla para compilar los objetos, creando carpetas si hace falta
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re