#include "Parser.hpp"

int main(void) {
	std::string path = "/Users/lzipp/Desktop/Webserv/config/webserv.conf";
	std::cout << "Parsing config file: " << path << std::endl;
	Parser parser(path);

	std::vector<ServerConfig> configs;
	try {
		configs = parser.ParseConfig();
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	std::cout << "Number of server configs: " << configs.size() << std::endl;
	if (configs.size() != 2) {
		std::cerr << "Error: No server configs found" << std::endl;
		return 1;
	}
	
	return 0;
}
