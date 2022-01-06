NAME = Durex

SRC_NAME = commands.c daemon.c durex.c hash.c main.c signal.c util.c
OBJ_NAME = $(SRC_NAME:.c=.o)

SRC_PATH = ./src/
OBJ_PATH = ./obj/

CC = gcc

HDR_PATH = -Iinc

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

$(NAME): $(SRC) $(INC)
	$(CC) -o $(NAME) $(SRC) $(HDR_PATH)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
