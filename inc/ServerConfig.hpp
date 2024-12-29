#pragma once

#include <map>

class ServerConfig {
    public:
        // TODO
    private:
        // maps server names to their ports
        std::map<std::string, uint> _serverPorts;
        // maps HTTP error status codes to their error pages
        // [...]
}