NAME = libft.a

CC = cc
CFLAGS = -g -Wall -Wextra -Werror

# Libft source files
LIBFT_SRC = ft_atoi.c \
            ft_atoll.c \
            ft_bzero.c \
            ft_calloc.c \
            ft_isalnum.c \
            ft_isalpha.c \
            ft_isascii.c \
            ft_isdigit.c \
            ft_isprint.c \
            ft_itoa.c \
            ft_memchr.c \
            ft_memcmp.c \
            ft_memcpy.c \
            ft_memmove.c \
            ft_memset.c \
            ft_putchar_fd.c \
            ft_putendl_fd.c \
            ft_putnbr_fd.c \
            ft_putstr_fd.c \
            ft_realloc.c \
            ft_substr.c \
            ft_split.c \
            ft_strchr.c \
            ft_striteri.c \
            ft_strjoin.c \
            ft_strcmp.c \
            ft_strdup.c \
            ft_strlcat.c \
            ft_strlcpy.c \
            ft_strlen.c \
            ft_strmapi.c \
            ft_strncmp.c \
            ft_strnstr.c \
            ft_strrchr.c \
            ft_strtrim.c \
            ft_tolower.c \
            ft_toupper.c \
            ft_isspace.c \
            ft_strncpy.c \
            ft_strcpy.c \
            ft_print_bits_int.c \
            ft_is_upper_alpha.c \
            ft_is_lower_alpha.c \
            ft_atoi_base.c \
            ft_parse_char_2d_arr.c \
            ft_parse_char_arr.c \
            ft_is_char_arr_solid.c \
            ft_free_arrstrs.c \
            ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
            ft_lstsize_bonus.c ft_lstlast_bonus.c \
            ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
            ft_lstclear_bonus.c ft_lstiter_bonus.c \
            ft_lstmap_bonus.c

# ft_printf source files
PRINTF_SRC = ft_printf/ft_printf.c \
             ft_printf/ft_printf_utils.c

# ft_printf source files
GET_N_LINE_SRC = get_next_line/get_next_line.c \
                get_next_line/get_next_line_utils.c

# Combine all source files
SRC = $(LIBFT_SRC) $(PRINTF_SRC) $(GET_N_LINE_SRC)

# Object files directory
OBJ_DIR = obj

# Create object file paths
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Header file
HEADER = libft.h

# Default rule
all: $(NAME)

# Create the library
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)
	@echo "\033[32m\"$(NAME)\": successfully created!\033[0m"

# Rule to compile source files into object files in the obj directory
$(OBJ_DIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@) # Ensure subdirectories exist
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Phony targets
.PHONY: all clean fclean re

# Clean object files and directory
clean:
	rm -rf $(OBJ_DIR)
	@echo -e "\033[32m\"$(NAME)\": temporary files successfully removed!\033[0m"
	@echo ""

# Clean all generated files (including the library)
fclean: clean
	rm -f $(NAME)
	@echo -e "\033[32m\"$(NAME)\": executable successfully removed!\033[0m"
	@echo ""

# Rebuild the project
re: fclean all
