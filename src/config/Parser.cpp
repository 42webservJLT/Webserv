#include "Parser.hpp"

// @formatter:on

bool _readServerBlock(std::string& serverBlock, const std::string& firstLine, std::ifstream& file);

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
		trim(line);
		if (line.size() >= 7 && line.substr(0, 7) == "server ") {
			// std::cout << "Found server block" << std::endl;
//			read in the server block
			try {
				// std::cout << "Reading server block" << std::endl;
				std::string serverBlock;
				if (!_readServerBlock(serverBlock, line, file)) {
					throw std::runtime_error("Error: Invalid server block");
				}
				// std::cout << serverBlock << std::endl;

				ServerConfig config;
				if (!config.Unmarshall(serverBlock, file)) {
					std::cout << "Error: Invalid server config" << std::endl;
					throw std::runtime_error("Error: Invalid server config");
				}
				configs.push_back(config);

				// configs.push_back(config);
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

/* ----------------------------------------------------------------------------------- */
/* Helpers                                                                             */
/* ----------------------------------------------------------------------------------- */
// reads a server block from the file
bool _readServerBlock(std::string& serverBlock, const std::string& firstLine, std::ifstream& file) {
	size_t leftBrackets = 0;
	size_t rightBrackets = 0;

	if (firstLine[firstLine.size() - 1] == '{') {
		++leftBrackets;
	}

	serverBlock = firstLine + "\n";
	std::string line;
	while (std::getline(file, line)) {
		trim(line);
		if (line.find("{") != std::string::npos) {
			++leftBrackets;
		} else if (line.find("}") != std::string::npos) {
			++rightBrackets;
		}

		if (leftBrackets == rightBrackets) {
			serverBlock += line;
			break;
		} else {
			serverBlock += line + "\n";
		}
	}

	std::cout << "Server block: " << serverBlock << std::endl;
	return true;
}
