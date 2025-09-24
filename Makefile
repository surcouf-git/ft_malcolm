CC = cc

UTILS = src/utils/global_utils.c \

SRC = src/main.c \
	  src/parsing/input.c \
	  src/main_loop/main_loop.c

INCLUDES = -I include/structs -I include/utils -I include/def -I include/input -I include/main_loop

RED=\e[31m
DEF=\e[0m

all:
	@echo "${RED}\nWARNING WERROR IS NOT SET AND -w IS SET${DEF}\n";
	gcc -g -Wall -Wextra  ${INCLUDES} $(UTILS) $(SRC) -o ft_malcolm

fclean:
	rm -f ft_malcolm

re: fclean all

.PHONY: all flcean re