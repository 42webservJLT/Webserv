#include "ServerConfig.hpp"

/* ----------------------------------------------------------------------------------- */
/* Constructor & Destructor                                                            */
/* ----------------------------------------------------------------------------------- */
RouteConfig::RouteConfig(
    const std::string& path,
    const std::string& root,
    const std::vector<std::string>& index,
    const std::set<std::string>& allowedMethods,
    bool autoindex,
    const std::string& redirect,
    const std::map<std::string, std::string>& cgiExtensions,
    const std::string& uploadDir,
    size_t clientMaxBodySize
) {
    path = path;
    root = root;
    index = index;
    allowedMethods = allowedMethods;
    autoindex = autoindex;
    redirect = redirect;
    cgiExtensions = cgiExtensions;
    uploadDir = uploadDir;
    clientMaxBodySize = clientMaxBodySize;
}

RouteConfig::~RouteConfig() {}

ServerConfig::ServerConfig(
    const std::string& host,
    uint16_t port,
    const std::vector<std::string>& serverNames,
    size_t clientMaxBodySize,
    std::map<uint16_t, std::string> errorPages,
    std::map<std::string, RouteConfig> routes
) {
    _host = host;
    _port = port;
    _serverNames = serverNames;
    _clientMaxBodySize = clientMaxBodySize;
    _errorPages = errorPages;
    _routes = routes;
}

ServerConfig::~ServerConfig() {}

/* ----------------------------------------------------------------------------------- */
/* Setters                                                                             */
/* ----------------------------------------------------------------------------------- */
void ServerConfig::SetHost(const std::string& host) {
    _host = host;
}

void ServerConfig::SetPort(uint16_t port) {
    _port = port;
}
void ServerConfig::AddServerName(const std::string& name) {
    _serverNames.push_back(name);
}

void ServerConfig::SetClientMaxBodySize(size_t size) {
    _clientMaxBodySize = size;
}

void ServerConfig::AddErrorPage(uint16_t code, const std::string& path) {
    _errorPages[code] = path;
}

void ServerConfig::AddRoute(const std::string& path, const RouteConfig& config) {
    _routes[path] = config;
}

/* ----------------------------------------------------------------------------------- */
/* Getters                                                                             */
/* ----------------------------------------------------------------------------------- */
const std::string& ServerConfig::GetHost() const {
    return _host;
}
uint16_t ServerConfig::GetPort() const {
    return _port;
}

const std::vector<std::string>& ServerConfig::GetServerNames() const {
    return _serverNames;
}

size_t ServerConfig::GetClientMaxBodySize() const {
    return _clientMaxBodySize;
}

const std::map<uint16_t, std::string>& ServerConfig::GetErrorPages() const {
    return _errorPages;
}

const std::string& ServerConfig::GetErrorPage(uint16_t code) const {
    return _errorPages.at(code);
}

const std::map<std::string, RouteConfig>& ServerConfig::GetRoutes() const {
    return _routes;
}

const RouteConfig& ServerConfig::GetRoute(const std::string& path) const {
    return _routes.at(path);
}
