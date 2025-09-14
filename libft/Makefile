AR = ar rcs
SRCS =		ft_strlen.c		ft_strdup.c		\
			ft_strjoin.c	ft_memcpy.c		\
			ft_substr.c		ft_strncpy.c	\
			ft_isalpha.c	ft_strcmp.c		\
			ft_isdigit.c	ft_isalnum.c	\
			ft_itoa.c

OBJ = ${SRCS:.c=.o}

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OBJ}
	${AR} ${NAME} ${OBJ}

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ${OBJ}

fclean : clean 
	rm -f ${NAME}
	
re: fclean all
