ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

PATH_INC = inc
PATH_SRC = src
PATH_TEST = test

SRC += $(wildcard $(PATH_SRC)/*.c)
SRC_TEST += $(wildcard $(PATH_TEST)/*.c)
SRC_TEST += $(wildcard $(PATH_SRC)/*.c)

OBJ = $(SRC:%.c=%.o)
OBJ_TEST = $(SRC_TEST:%.c=%.o)

# **************************************************************************** #
# VARIABLES         														   #
# **************************************************************************** #

NAME = libft_malloc_$(HOSTTYPE).so
LIB_NAME = libft_malloc.so
NAME_TEST = testm

HISTORY_FILEPATH = alloc_history.txt

CC = gcc

FLAGS_CC = -Wall -Wextra -Werror -fPIC
FLAGS_TEST = -Wall -Wextra -Werror
FLAGS_LIB = -shared

# **************************************************************************** #
# COMMANDS  		    													   #
# **************************************************************************** #

%.o : %.c
	$(CC) $(FLAGS_CC) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS_LIB) -o $@ $(OBJ)
	rm -f $(LIB_NAME)
	ln -s $(NAME) $(LIB_NAME)

$(NAME_TEST): $(OBJ_TEST)
	$(CC) $(FLAGS_TEST) -o $(NAME_TEST) $(OBJ_TEST)

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.c
	mkdir -p $(@D)
	$(CC) -c -o $@ $(FLAGS_CC) $^ -O0 -g -I $(PATH_INC)

clean:
	rm -rf $(OBJ) $(OBJ_TEST)

fclean: clean
	rm -rf $(NAME) $(NAME_TEST) $(LIB_NAME) $(HISTORY_FILEPATH) 

re: fclean $(NAME)

.PHONY: all clean fclean re
