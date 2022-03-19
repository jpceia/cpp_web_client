# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/04 23:37:29 by jceia             #+#    #+#              #
#    Updated: 2022/03/19 03:31:47 by jpceia           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = https-webserv

INC_DIR     = inc
SRC_DIR     = src
OBJ_DIR     = obj
BIN_DIR	 	= bin

SRCS        = $(shell find $(SRC_DIR) -name "*.cpp" -type f)
OBJS        = $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)

CXX          = clang++
RM           = rm -f

OS          = $(shell uname)

FLAGS_WARN  = -Wall -Werror -Wextra -pedantic-errors
FLAGS_INC   = -I$(INC_DIR)
FLAGS_DEBUG = -g -fsanitize=address -DDEBUG
# sudo apt-get install libssl-dev
FLAGS_LIBS  = -lssl -lcrypto
FLAGS_OPT   = -O3

# Compilation flags
CXXFLAGS    = $(FLAGS_WARN) $(FLAGS_INC) -std=c++98

# Linker flags
LDFLAGS     = $(FLAGS_WARN) $(FLAGS_LIBS) -std=c++98

# Compilation
$(OBJ_DIR)/%.o:    $(SRC_DIR)/%
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

all: $(NAME)

# Cleaning
clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

# Rebuild
re: fclean all

# Debug build
debug:      CXXFLAGS += $(FLAGS_DEBUG)
debug:      all


.PHONY:     all clean fclean re debug