NAME     = minishell
CC       = cc 
INCLUDES = -I include
CFLAGS   = $(INCLUDES) -g -fsanitize=address
# CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
LIBS     = -lreadline
SRCS     = src/main.c src/readline.c src/tokenize.c src/exec.c src/utils.c src/destructor.c src/error.c src/expand.c
OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)
clean:
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re test
##########################