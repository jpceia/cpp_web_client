# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/04 23:37:29 by jceia             #+#    #+#              #
#    Updated: 2022/03/27 06:54:15 by jpceia           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = cpp_web_client

INC_DIR     = inc
SRC_DIR     = src
OBJ_DIR     = obj
BIN_DIR	 	= bin

SHARED_SRCS = $(shell find $(SRC_DIR) -mindepth 2 -name "*.cpp" -type f) # 
SHARED_OBJS = $(SHARED_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

MAIN_SRCS   = $(shell find $(SRC_DIR) -maxdepth 1 -name "*.cpp" -type f)
MAIN_OBJS	= $(MAIN_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
BINS		= $(MAIN_SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%)

SRCS 	  = $(SHARED_SRCS) $(MAIN_SRCS)
OBJS 	  = $(SHARED_OBJS) $(MAIN_OBJS)

CXX          = clang++
RM           = rm -f

OS          = $(shell uname)

FLAGS_WARN  = -Wall -Werror -Wextra -pedantic-errors
FLAGS_INC   = -I$(INC_DIR)
FLAGS_DEBUG = -ggdb3 -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC # -fsanitize=address
# sudo apt-get install libssl-dev
FLAGS_LIBS  = -lssl -lcrypto
FLAGS_OPT   = -O3
FLAGS_DEPS	= -MD

# Compilation flags
CXXFLAGS    = $(FLAGS_WARN) $(FLAGS_DEPS) $(FLAGS_INC) -std=c++98

# Linker flags
LDFLAGS     = $(FLAGS_WARN) $(FLAGS_LIBS) -std=c++98

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/%: $(SHARED_OBJS) $(OBJ_DIR)/%.o
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ -o $@

all: $(BINS)

# Cleaning
clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) -rf $(BIN_DIR)

# Rebuild
re: fclean all

# Debug build
debug:      CXXFLAGS += $(FLAGS_DEBUG)
debug:		LDFLAGS += $(FLAGS_DEBUG)
debug:      all


.PHONY:     all clean fclean re debug