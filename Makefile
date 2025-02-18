NAME 	= ft_ls

CC 		= clang
CFLAGS 	= -Wall -Werror -Wextra -g3 -fsanitize=address
INC 	= -I inc -I libft/include
LIB		= -L libft -lft
RM		= rm -rf

FILES	= \
	src/main

SRC 	= $(addsuffix .c, $(FILES))
OBJ 	= $(addsuffix .o, $(FILES))

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

$(NAME): $(OBJ)
	make -C libft
	@echo "\033[1;33m\t\tCompiling all source files into unique binary\033[0;m"
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIB)

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	@echo "\033[1;31m\t\tRemoving $(NAME)\033[0;m"
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
