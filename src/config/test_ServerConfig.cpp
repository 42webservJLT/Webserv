#include "Parser.hpp"
#include <iostream>

bool _lineValid(std::string& line);
bool _handleHost(std::string& line, std::string& host);
bool _handlePort(std::string& line, uint16_t& port);
bool _handleServerName(std::string& line, std::vector<std::string>& serverNames);
bool _handleErrorPage(std::string& line, std::map<HttpStatus, std::string>& errorPages);
bool _handleClientMaxBodySize(std::string& line, size_t& clientMaxBodySize);
bool _handleLocation(std::string& line, std::ifstream& file, std::map<std::string, RouteConfig>& routes);
bool _readRouteBlock(const std::string& firstLine, std::ifstream& file, std::string& routeBlock);

bool test_lineValid() {
	try {
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
				std::cout << "Failed on valid line: |" << line << "|" << std::endl;
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
		invalids.push_back("server_name localhost; server_name localhost;");
		invalids.push_back("server_name localhost; server_name localhost; server_name localhost;");
		invalids.push_back("server_name localhost");
		for (std::string& line : invalids) {
			if (_lineValid(line)) {
				std::cout << "Failed on invalid line: " << line << std::endl;
				return false;
			}
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleHost() {
	try {
		std::string valid = "host localhost;";
		std::string invalid = "host localhost; port 8080;";
		std::string host;
		if (!_handleHost(valid, host)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (host != "localhost") {
			std::cout << "Failed on host value: " << host << std::endl;
			return false;
		}

		if (_handleHost(invalid, host)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handlePort() {
	try {
		std::string valid = "port 8080;";
		std::string invalid = "port 8080; host localhost;";
		uint16_t port;
		if (!_handlePort(valid, port)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (port != 8080) {
			std::cout << "Failed on port value: " << port << std::endl;
			return false;
		}

		if (_handlePort(invalid, port)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleServerName() {
	try {
		std::string valid = "server_name localhost;";
		std::string valid2 = "server_name localhost www.lzipp.de www.lzipp.com;";
		std::string invalid = "server_name;";
		std::vector<std::string> serverNames;
		if (!_handleServerName(valid, serverNames)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (serverNames.size() != 1 || serverNames[0] != "localhost") {
			std::cout << "Failed on server name value: " << serverNames[0] << std::endl;
			return false;
		}

		serverNames.clear();
		if (!_handleServerName(valid2, serverNames)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (serverNames.size() != 3 || serverNames[0] != "localhost" || serverNames[1] != "www.lzipp.de" || serverNames[2] != "www.lzipp.com") {
			std::cout << "---\n";
			std::cout << "Failed on server name value: " << serverNames[0] << std::endl;
			std::cout << "Failed on server name value: " << serverNames[1] << std::endl;
			std::cout << "Failed on server name value: " << serverNames[2] << std::endl;
			return false;
		}

		serverNames.clear();
		if (_handleServerName(invalid, serverNames)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleErrorPage() {
	try {
		std::string valid = "error_page 404 /404.html;";
		std::string invalid = "error_page 404;";
		std::string invalid2 = "error_page 404 /404.html /405.html;";
		std::string invalid3 = "error_page 404 /404.html; host localhost;";

		std::map<HttpStatus, std::string> errorPages;
		if (!_handleErrorPage(valid, errorPages)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (errorPages.size() != 1 || errorPages[HttpStatus::NOT_FOUND] != "/404.html") {
			std::cout << "Failed on error page value: " << errorPages[HttpStatus::NOT_FOUND] << std::endl;
			return false;
		}

		if (_handleErrorPage(invalid, errorPages)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		} else if (_handleErrorPage(invalid2, errorPages)) {
			std::cout << "Failed on invalid line: " << invalid2 << std::endl;
			return false;
		} else if (_handleErrorPage(invalid3, errorPages)) {
			std::cout << "Failed on invalid line: " << invalid3 << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleClientMaxBodySize() {
	try {
		std::string valid = "client_max_body_size 1333;";
		std::string valid2 = "client_max_body_size 200000;";
		std::string invalid = "client_max_body_size 1MB;";
		std::string invalid2 = "client_max_body_size abcdef;";
		size_t clientMaxBodySize;
		if (!_handleClientMaxBodySize(valid, clientMaxBodySize)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (clientMaxBodySize != 1333) {
			std::cout << "Failed on client max body size value: " << clientMaxBodySize << std::endl;
			return false;
		}

		if (!_handleClientMaxBodySize(valid2, clientMaxBodySize)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (clientMaxBodySize != 200000) {
			std::cout << "Failed on client max body size value: " << clientMaxBodySize << std::endl;
			return false;
		}

		if (_handleClientMaxBodySize(invalid, clientMaxBodySize)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
		if (_handleClientMaxBodySize(invalid2, clientMaxBodySize)) {
			std::cout << "Failed on invalid line: " << invalid2 << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

// TODO: Implement test_handleLocation
bool test_handleLocation() {
	return true;
}

// @formatter:on
int main(void) {
	// std::string path = "/Users/lzipp/Desktop/Webserv/config/webserv.conf";
	// std::cout << "Parsing config file: " << path << std::endl;
	// Parser parser(path);

	// std::vector<ServerConfig> configs = parser.ParseConfig();

	int exitCode = 0;

	if (!test_lineValid()) {
		std::cout << "Failed test_lineValid" << std::endl;
		exitCode = 1;
	} 
	if (!test_handleHost()) {
		std::cout << "Failed test_handleHost" << std::endl;
		exitCode = 1;
	} 
	if (!test_handlePort()) {
		std::cout << "Failed test_handlePort" << std::endl;
		exitCode = 1;
	} 
	if (!test_handleServerName()) {
		std::cout << "Failed test_handleServerName" << std::endl;
		exitCode = 1;
	} 
	if (!test_handleErrorPage()) {
		std::cout << "Failed test_handleErrorPage" << std::endl;
		exitCode = 1;
	} 
	if (!test_handleClientMaxBodySize()) {
		std::cout << "Failed test_handleClientMaxBodySize" << std::endl;
		exitCode = 1;
	}
	if (!test_handleLocation()) {
		std::cout << "Failed test_handleLocation" << std::endl;
		exitCode = 1;
	}

	return 0;
}
