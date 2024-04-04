NAME       = dev_pipe
CC         = cc
CFLAGS     = -Wall -Wextra -Werror
DFLAGS     = -g -fsanitize=address
LIBRARY    = -L$(LIBFT_DIR) -lft
INCLUDE    = -I$(HDR_DIR) -I$(LIBFT_DIR)includes/
LIBFT      = $(LIBFT_DIR)libft.a
LIBFT_DIR  = ./libft/
HDR_LIST   = *.h
HDR_DIR    = ./includes/
HDR        = $(addprefix $(HDR_DIR), $(HDR_LIST))
SRCS       = *.c
SRCS_DIR   = ./srcs/
OBJ_DIR    = ./obj/
OBJS       = $(addprefix $(OBJ_DIR),$(SRCS:.c=.o))

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBRARY) $(LDFLAGS) $(INCLUDE)

debug: $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(NAME) $(LIBRARY) $(LDFLAGS) $(INCLUDE)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(MLX):
	make -C mlx

all: $(NAME)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c $(HDR)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(LIBFT)
	make -C $(LIBFT_DIR) fclean

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
