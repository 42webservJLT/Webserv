NAME := webserv

CPP := c++
CPPFLAGS := -Wextra -Wall -Werror -std=c++17 -I./inc
TESTFLAGS := -Wextra -Wall -std=c++17 -I./inc

SRCDIR := ./src
SRC += $(addprefix $(SRCDIR)/config/, ServerConfig.cpp RouteConfig.cpp Parser.cpp)
SRC += $(addprefix $(SRCDIR)/tcpServer/, TCPServer.cpp)
SRC += $(addprefix $(SRCDIR)/, helpers.cpp main.cpp)

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

tall: tserv troute tparser

tserv:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/helpers.cpp $(SRCDIR)/config/Parser.cpp $(SRCDIR)/config/RouteConfig.cpp $(SRCDIR)/config/ServerConfig.cpp tests/config/test_ServerConfig.cpp -o ServerTest \
	&& ./ServerTest

troute:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/helpers.cpp $(SRCDIR)/config/RouteConfig.cpp tests/config/test_RouteConfig.cpp -o RouteTest \
	&& ./RouteTest

tparser:
	$(CPP) $(TESTFLAGS) $(SRCDIR)/helpers.cpp $(SRCDIR)/config/RouteConfig.cpp $(SRCDIR)/config/ServerConfig.cpp $(SRCDIR)/config/Parser.cpp tests/config/test_Parser.cpp -o ParserTest \
	&& ./ParserTest

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: all clean fclean re lint tall tserv troute tparser