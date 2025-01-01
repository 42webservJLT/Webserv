#include "Parser.hpp"
#include <iostream>

// @formatter:on
int main(void) {
	std::string path = "/Users/lzipp/Desktop/Webserv/config/webserv.conf";
    std::cout << "Parsing config file: " << path << std::endl;
	Parser parser(path);

	std::vector<ServerConfig> configs = parser.ParseConfig();

	return 0;
}
