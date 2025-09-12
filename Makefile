CC = cc
SRC = src/main.c

all:
	cc -g -Wall -Wextra -Werror $(SRC) -o ft_malcolm

fclean:
	rm -f ft_malcolm

re: fclean all

.PHONY: all flcean re