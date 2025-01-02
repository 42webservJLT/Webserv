#include "Parser.hpp"
#include <iostream>

bool _lineValid(std::string& line);
bool _handleHost(std::string& line, std::string& host);
bool _handlePort(std::string& line, uint16_t& port);
bool _handleServerName(std::string& line, std::vector<std::string>& serverNames);
bool _handleErrorPage(std::string& line, std::map<uint16_t, std::string>& errorPages);
bool _handleClientMaxBodySize(std::string& line, size_t& clientMaxBodySize);
bool _handleLocation(std::string& line, std::map<std::string, RouteConfig>& routes);

bool test_lineValid() {
	// valids
	std::vector<std::string> valids;
	valids.push_back("server {");
	valids.push_back("server { ");
	valids.push_back("}");
	valids.push_back("port 8080;");
	valids.push_back("server_name localhost;");
	valids.push_back("error_page 404 /404.html;");
	for (std::string& line : valids) {
		if (!_lineValid(line)) {
			std::cout << "Failed on valid line: " << line << std::endl;
			return false;
		}
	}

	// technically valid, but errors when checked by _handle... functions
	std::string l7 = "server_name localhost; server_name localhost;";
	std::string l8 = "server_name localhost; server_name localhost; server_name localhost;";
	std::vector<std::string> technicallyValids;
	technicallyValids.push_back("server_name localhost; server_name localhost;");
	technicallyValids.push_back("server_name localhost; server_name localhost; server_name localhost;");
	for (std::string& line : technicallyValids) {
		if (!_lineValid(line)) {
			std::cout << "Failed on technically valid line: " << line << std::endl;
			return false;
		}
	}

	// invalids
	std::vector<std::string> invalids;
	invalids.push_back("server");
	invalids.push_back("{");
	invalids.push_back("};");
	invalids.push_back("port 8080");
	invalids.push_back(";");
	for (std::string& line : invalids) {
		if (_lineValid(line)) {
			std::cout << "Failed on invalid line: " << line << std::endl;
			return false;
		}
	}

	return true;
}

// @formatter:on
int main(void) {
	std::string path = "/Users/lzipp/Desktop/Webserv/config/webserv.conf";
	std::cout << "Parsing config file: " << path << std::endl;
	Parser parser(path);

	std::vector<ServerConfig> configs = parser.ParseConfig();

	return 0;
}
