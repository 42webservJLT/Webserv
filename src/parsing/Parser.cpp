#include "Parser.hpp"

// @formatter:on

/* ----------------------------------------------------------------------------------- */
/* Helpers                                                                             */
/* ----------------------------------------------------------------------------------- */

std::string _readServerBlock(const std::string& firstLine, std::ifstream& file) {
	size_t leftBrackets = 0;
	size_t rightBrackets = 0;

	if (firstLine.find("{") != std::string::npos) {
		leftBrackets++;
	}

	std::string serverBlock = firstLine + "\n";
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("{") != std::string::npos) {
			leftBrackets++;
		} else if (line.find("}") != std::string::npos) {
			rightBrackets++;
		}

		if (leftBrackets == rightBrackets) {
			serverBlock += line;
			break;
		} else {
			serverBlock += line + "\n";
		}
	}
	return serverBlock;
}

/* ----------------------------------------------------------------------------------- */
/* Parser Constructor & Destructor                                                     */
/* ----------------------------------------------------------------------------------- */
Parser::Parser(const std::string path) : _path(path) {}

Parser::Parser(const Parser& other) {
	*this = other;
}

Parser::~Parser() {}

Parser& Parser::operator=(const Parser& other) {
	if (this != &other) {
		_path = other._path;
	}
	return *this;
}

/* ----------------------------------------------------------------------------------- */
/* Parser Parsing                                                                      */
/* ----------------------------------------------------------------------------------- */
// Parses the config file and returns an array of ServerConfig objects
std::vector<ServerConfig> Parser::ParseConfig() {
	std::vector<ServerConfig> configs;
	std::ifstream file;

	file.open(_path);
	if (!file.is_open()) {
		throw std::runtime_error("Error: Could not open file");
	}

	std::string line;
	while (std::getline(file, line)) {
//		when a server is encountered, parse the server config
		if (line.find("server") != std::string::npos) {
			std::cout << "Found server block" << std::endl;
//			read in the server block
			try {
				std::string serverBlock = _readServerBlock(line, file);
				std::cout << serverBlock << std::endl;
//				ServerConfig config = ParseServerConfig(serverBlock);
//				configs.push_back(config);
			} catch (std::exception& e) {
				file.close();
				throw e;
			}
		}
	}

//	remember to do this whenever function returns
	file.close();

	return configs;
}
