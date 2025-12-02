# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lde-medi <lde-medio@student.42madrid.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/09 20:22:59 by lde-medi          #+#    #+#              #
#    Updated: 2025/12/02 03:16:31 by lde-medi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = editor_cub3d
CC = c++
CFLAGS = -g3 -Iincludes -Isfmlgui
SRCS = main.cpp $(GUI_SRCS) srcs/MapData.cpp
GUI_SRCS = $(wildcard sfmlgui/*.cpp)
OBJS = $(SRCS:.cpp=.o)
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lGL

GREEN = \033[0;32m
DEFAULT = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo -e "$(GREEN)$(NAME) created!$(DEFAULT)"

%.o: %.cpp 
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@echo -e "$(GREEN)$(NAME) obj files deleted!$(DEFAULT)"

fclean: clean
	@rm -f $(NAME)
	@echo -e "$(GREEN)$(NAME) all files deleted!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re
