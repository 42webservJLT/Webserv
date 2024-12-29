#pragma once

#include <string>

#include "HTTPStatus.hpp"
#include "ServerConfig.hpp"

class Parser {
    public:
        Parser(std::string path);
        Parser(const Parser& other);
        ~Parser();
        Parser& operator=(const Parser& other);

        // parses the config file and returns a Config object
        Config& Parse();
        // [...]
    private:
        // TODO
}
