# **************************************************************************** #
#                               LIBFT MAKEFILE                                 #
# **************************************************************************** #

# Library name / Nombre de la biblioteca
NAME = libft.a

# Directory and library for ft_dprintf / Directorio y librería de ft_dprintf
DPRINTF_DIR = ft_dprintf
DPRINTF_LIB = $(DPRINTF_DIR)/libftdprintf.a

# -----------------------------------------------------------------------------
# COMPILATION / COMPILACIÓN
# -----------------------------------------------------------------------------

# Compiler and flags / Compilador y banderas
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -Ignl -I$(DPRINTF_DIR)
AR = ar rcs

# -----------------------------------------------------------------------------
# SOURCE FILES / ARCHIVOS FUENTE
# -----------------------------------------------------------------------------

# Core source files / Archivos fuente principales
SRCS = \
	ft_arraylen.c ft_atoi.c ft_atol.c ft_bzero.c ft_calloc.c \
	ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	ft_isspace.c ft_isquote.c ft_issymbol.c ft_itoa.c ft_isnumeric.c ft_isposfdstr.c \
	ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
	ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
	ft_split.c ft_strchr.c ft_strdel.c ft_strdup.c ft_strldup.c \
	ft_striteri.c ft_strjoin.c ft_strjoin_free.c ft_strlcat.c \
	ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strcmp.c ft_strcpy.c \
	ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
	ft_tolower.c ft_toupper.c ft_printf.c ft_putchar.c ft_puthex.c \
	ft_putnbr.c ft_putptr.c ft_putstr.c ft_putunsigned.c ft_strcat.c \
	get_next_line.c ft_realloc.c ft_strtab.c ft_strdup_array.c \
	gnl/get_next_line.c gnl/get_next_line_utils.c

# Object files / Archivos objeto
OBJS = $(SRCS:.c=.o)

# Bonus source files / Archivos fuente adicionales (bonus)
BONUS_SRCS = \
	ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
	ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c

# Bonus object files / Archivos objeto del bonus
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# -----------------------------------------------------------------------------
# RULES / REGLAS
# -----------------------------------------------------------------------------

# Default rule: build the library / Regla por defecto: compilar la biblioteca
all: $(DPRINTF_LIB) $(NAME)

# Build libft.a / Compilar libft.a
$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)
	@echo "\033[32m✔ libft.a successfully built / compilada con éxito\033[0m"

# Build dprintf library / Compilar la librería dprintf
$(DPRINTF_LIB):
	$(MAKE) -C $(DPRINTF_DIR)

# Compile object files / Compilar archivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------
# CLEANING / LIMPIEZA
# -----------------------------------------------------------------------------

# Clean object files / Limpiar archivos .o
clean:
	rm -f $(OBJS) $(BONUS_OBJS)
	$(MAKE) -C $(DPRINTF_DIR) clean
	@echo "\033[33m🧹 Cleaned object files / objetos limpiados\033[0m"

# Clean all (object files and libraries) / Limpiar todo (objetos y librerías)
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(DPRINTF_DIR) fclean
	@echo "\033[31m💥 libft.a removed / eliminada\033[0m"

# Rebuild from scratch / Reconstruir desde cero
re: fclean all

# Avoid conflicts with file names / Evitar conflictos con nombres de archivos
.PHONY: all clean fclean re
