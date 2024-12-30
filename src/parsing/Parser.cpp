#include "Parser.hpp"

/* ----------------------------------------------------------------------------------- */
/* Parser Constructor & Destructor                                                     */
/* ----------------------------------------------------------------------------------- */
Parser::Parser(std::string path) : _path(path) {}

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
std::vector<ServerConfig>& Parser::ParseConfig() {
    std::vector<ServerConfig> configs;
    std::ifstream file(_path);
    // TODO
}