
NAME=my_cat
CFLAGS=-Wall
INCLUDES=-I./inc
CC=gcc

SRCS=src/errExit.c src/main.c
OBJS=$(SRCS:.c=.o)

$(NAME): $(OBJS)
	@echo "Making executable: "$@
	@$(CC) $^ -o $@

.c.o:
	@echo "Compiling: "$<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean:
	@rm -f src/*.o $(NAME)
	@echo "Removed object files and executable..."
