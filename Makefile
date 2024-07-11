NAME		:= minishell
SRC_DIR		:= src
INC_DIR		:= include
OBJ_DIR		:= obj

SRC_LIST	:= *.c
SRCS		:= $(addprefix $(SRC_DIR)/, $(SRC_LIST))
OBJS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		:= $(OBJS:.o=.d)

INC			:= -I$(INC_DIR)
LIBS		:= -lreadline

CC			:= cc
DFLAGS		:= -g -fsanitize=address
CFLAGS		:= -Wall -Werror $(INC)
#CFLAGS		:= -Wall -Wextra -Werror $(INC)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: clean $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(LIBS) -o $(NAME) $(OBJS)

.PHONY: all clean fclean re debug

-include $(DEPS)
