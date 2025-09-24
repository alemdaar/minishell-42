CC = cc
READLINE_DIR = /Users/oelhasso/.brew/opt/readline

CFLAGS = -Wall -Wextra -Werror \
         -I includes/ \
         -I$(READLINE_DIR)/include \
         -g -fsanitize=address

LDFLAGS = -L$(READLINE_DIR)/lib \
          -lreadline -lhistory -lncurses

NAME = minishell

LIB = ./libft/libft.a

SRC = 	minishell.c \
		parsing/asm_cmd.c							\
		parsing/tokens_core/list_tokens.c			\
		parsing/tokens_core/refactor_tokens.c		\
		parsing/tokens_core/unquotes.c			\
		parsing/tokens_core/utils0_tokens.c		\
		parsing/tokens_core/utils1_tokens.c		\
		parsing/tokens_core/utils3_tokens.c		\
		parsing/envp/env_expander.c				\
		parsing/envp/env_heredoc.c				\
		parsing/envp/env_list.c					\
		parsing/envp/env_split.c					\
		parsing/envp/exp_special.c				    \
		parsing/envp/utils_env.c					\
		parsing/envp/utils1_env.c					\
		parsing/envp/clean_env.c					\
		parsing/cmd/list_cmd.c					\
		parsing/cmd/utils_cmd.c					\
		parsing/cmd/nodes_cmd.c					\
		parsing/cmd/content_cmd.c					\
		parsing/syntax/scrap.c					\
		parsing/syntax/utils_scrap.c				\
		parsing/leaks/ffree.c						\
		parsing/leaks/free_cmd_list.c				\
		parsing/signal/signal.c					\
		parsing/error/error.c		\
		exec/gnl/get_next_line.c					\
		exec/gnl/get_next_line_utils.c				\
		exec/linkedlist/linkedlist.c				\
		exec/bin.c									\
		exec/cd.c									\
		exec/child.c									\
		exec/cmd.c									\
		exec/echo.c									\
		exec/env.c									\
		exec/exec.c									\
		exec/exec2.c									\
		exec/exit_status.c									\
		exec/exit.c									\
		exec/export.c									\
		exec/file.c									\
		exec/file2.c									\
		exec/free.c									\
		exec/heredoc.c									\
		exec/paths.c									\
		exec/pwd.c									\
		exec/tools.c									\
		exec/tools2.c									\
		exec/tools3.c									\
		exec/unset.c									\
		exec/work.c									\
		exec/fds.c

HDR = minishell.h \
       ./parsing/include/token.h \

OBJ = $(SRC:.c=.o)

SRC_LIB = 	./libft/ft_strjoin.c 	\
			./libft/ft_strdup.c 	\
			./libft/ft_strlen.c 	\
			./libft/ft_memcpy.c		\
			./libft/ft_substr.c		\
			./libft/ft_strncpy.c	\
			./libft/ft_isalpha.c	\
			./libft/ft_isdigit.c	\
			./libft/ft_isalnum.c	\
			./libft/ft_strcmp.c		\

OBJ_LIB = $(SRC_LIB:.c=.o)
HDR_LIB = ./libft/libft.h

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@ $(LDFLAGS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(SRC_LIB) $(HDR_LIB)
	$(MAKE) -C ./libft

clean:
	rm -f $(OBJ)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all
