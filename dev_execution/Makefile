CC      = cc
CFLAGS  = -Wall -Wextra -Werror
LDFLAGS =
LIBS    =
INCLUDE = -I./

NAME    = exectest
SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)
HDRS    = $(wildcard *.h)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(NAME)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re