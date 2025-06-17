CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a

SRCS =		ft_strlen.c		ft_strdup.c		\
			ft_strjoin.c	ft_memcpy.c		\
			ft_substr.c		ft_strncpy.c	\
			ft_isalpha.c	ft_strcmp.c		\
			ft_isdigit.c	ft_isalnum.c	\

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
