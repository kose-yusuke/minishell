NAME		:= minishell
SRC_DIR		:= src
INC_DIR		:= include
INC			:= -I$(INC_DIR)
SRCS		:= $(addprefix $(SRC_DIR)/,main.c readline.c tokenize.c exec.c utils.c destructor.c error.c expand.c)
OBJS		:= $(SRCS:%.c=%.o)
LIBS		:= -lreadline
CC			:= gcc
DFLAGS		:= -g -fsanitize=address
CFLAGS		:= -Wall -Werror $(INC)
#CFLAGS		:= -Wall -Werror -Wextra $(INC)  tmp commented out

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: clean $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(LIBS) -o $(NAME) $(OBJS)

.PHONY: all clean fclean re debug
