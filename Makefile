NAME = malloc

SRCS =	src/main.c \
		src/malloc.c \
		src/logger.c \
		src/heap.c \
		src/blocks.c \
		src/utils.c

OBJS = ${SRCS:.c=.o}

LIB = src/libft/libft.a

CC = clang

CFLAGS = -g -Wall -Wextra

RM = rm -f

all		:	${NAME}

.c.o	:	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}	:	${OBJS}
		make -C src/libft
		${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIB}

libft	:
		make -C src/libft

clean	:
		make -C src/libft clean
		${RM} ${OBJS}

fclean	:
		make -C src/libft fclean
		${RM} ${OBJS}

re		: fclean all

.PHONY	: all clean fclean re libft