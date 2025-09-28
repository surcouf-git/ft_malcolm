NAME = ft_malcolm
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I include/structs -I include/utils -I include/def -I include/input -I include/main_loop
OBJS_DIR = objs

SRCS =	src/main.c \
		src/parsing/input.c \
		src/main_loop/main_loop.c \
		src/utils/global_utils.c \
		src/utils/display.c

OBJS = $(SRCS:src/%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c src/$*.c -o $(OBJS_DIR)/$*.o

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re