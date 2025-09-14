CC = cc
SRC = src/main.c

all:
	@echo "\n\nWARNING WERROR IS NOT SET\n\n";
	cc -g -Wall -Wextra $(SRC) -o ft_malcolm

fclean:
	rm -f ft_malcolm

re: fclean all

.PHONY: all flcean re