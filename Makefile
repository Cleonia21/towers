NAME	=	towers

CC		=	c++ -Wall -Wextra -std=c++17
LIBS	=	-I/usr/local/include/SDL2 -D_THREAD_SAFE -L/usr/local/lib -lSDL2 -lSDL2_image

HPP		=	Game.hpp \
			Map.hpp \


CPP		=	main.cpp \
			Game.cpp \
			Map.cpp \

OBJ_DIR	=	.objs/
OBJS	=	$(addprefix $(OBJ_DIR), $(CPP:.cpp=.o))

all: 		$(NAME)

$(OBJ_DIR):
			mkdir -p $@

$(OBJ_DIR)%.o: %.cpp $(HPP)
			$(CC) -c $< -o $@

$(NAME): $(OBJ_DIR) $(OBJS)
			$(CC) $(LIBS) $(OBJS) -o $(NAME)

clean:
			rm -rf $(OBJ_DIR)

fclean: clean
			rm -f $(NAME) $(MEBOT)

re: fclean all

.PHONY: all clean fclean re