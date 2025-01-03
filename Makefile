NAME := webserv

CPP := c++
CPPFLAGS := -Wextra -Wall -Werror -std=c++17 -I./inc
TESTFLAGS := -Wextra -Wall -std=c++17 -I./inc

SRCDIR := ./src
SRC += $(addprefix $(SRCDIR)/config/, ServerConfig.cpp RouteConfig.cpp Parser.cpp)
SRC := $(addprefix $(SRCDIR)/, main.cpp)

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

tserv:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/config/helpers.cpp $(SRCDIR)/config/Parser.cpp $(SRCDIR)/config/RouteConfig.cpp $(SRCDIR)/config/ServerConfig.cpp $(SRCDIR)/config/test_ServerConfig.cpp -o ServerTest

troute:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/config/helpers.cpp $(SRCDIR)/config/RouteConfig.cpp $(SRCDIR)/config/test_RouteConfig.cpp -o RouteTest

tparser:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/config/helpers.cpp $(SRCDIR)/config/RouteConfig.cpp $(SRCDIR)/config/ServerConfig.cpp $(SRCDIR)/config/Parser.cpp $(SRCDIR)/config/test_Parser.cpp -o ParserTest

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: all clean fclean re