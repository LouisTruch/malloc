ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

PATH_INC = inc
PATH_OBJ = obj
PATH_SRC = src

SRC += malloc.c free.c realloc.c utils.c lib.c ft_printf.c
SRC_TEST += main.c malloc.c free.c realloc.c utils.c lib.c ft_printf.c 

OBJ = $(SRC:%.c=$(PATH_OBJ)/%.o)
OBJ_TEST = $(SRC_TEST:%.c=$(PATH_OBJ)/%.o)

# **************************************************************************** #
# VARIABLES         														   #
# **************************************************************************** #

NAME = libft_malloc_$(HOSTTYPE).so
LIB_NAME = libft_malloc.so
NAME_TEST = malloc

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror -fPIC
FLAGS_TEST = -Wall -Wextra -Werror
FLAGS_LIB = -shared

# **************************************************************************** #
# COMMANDS  		    													   #
# **************************************************************************** #


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS_LIB) -o $@ $(OBJ)
	@rm -f $(LIB_NAME)
	ln -s $(NAME) $(LIB_NAME)
	@echo "Make done"

$(NAME_TEST): $(OBJ_TEST)
	$(CC) $(FLAGS_TEST) -o $(NAME_TEST) $(OBJ_TEST)

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $(FLAGS_CC) $^ -O0 -g -I $(PATH_INC)

clean:
	@rm -rf $(PATH_OBJ)
	@echo "Clean done"

fclean: clean
	@rm -f $(NAME) $(NAME_TEST) $(LIB_NAME)
	@echo "Fclean done"

re: fclean $(NAME)

.PHONY: all clean fclean re
