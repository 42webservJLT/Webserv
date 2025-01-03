#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "helpers.hpp"
#include "HTTPStatus.hpp"
#include "ServerConfig.hpp"

class Parser {
    public:
        Parser(std::string path);
        Parser(const Parser& other);
        ~Parser();
        Parser& operator=(const Parser& other);

        std::vector<ServerConfig> ParseConfig();
        // [...]
    private:
        std::string _path;
};
