NAME := webserv

CPP := c++
CPPFLAGS := -Wextra -Wall -Werror -std=c++17 -I./inc

SRCDIR := ./src
# [...]
SRC := $(addprefix $(SRCDIR)/, main.cpp)
# SRC += [...]
OBJDIR = ./obj
OBJ := $(addprefix $(OBJDIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJDIR)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

lint:
	cppcheck --error-exitcode=1 --enable=all --suppress=missingInclude ./src
	find ./inc -type f -name "*.hpp" -exec cppcheck --error-exitcode=1 --enable=all --suppress=missingInclude {} \;

tparse:
	echo TODO

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: all clean fclean re