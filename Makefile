NAME = malloc

SRCS =	src/malloc.c \

OBJS = ${SRCS:.c=.o}

LIB = src/libft/libft.a

CC = clang

CFLAGS = -Wall -Werror -Wextra -g

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