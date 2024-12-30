#include "ServerConfig.hpp"

/* ----------------------------------------------------------------------------------- */
/* ServerConfig Constructor & Destructor                                               */
/* ----------------------------------------------------------------------------------- */
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

ServerConfig::ServerConfig(const ServerConfig& other) {
    *this = other;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
    if (this != &other) {
        _host = other._host;
        _port = other._port;
        _serverNames = other._serverNames;
        _clientMaxBodySize = other._clientMaxBodySize;
        _errorPages = other._errorPages;
        _routes = other._routes;
    }
    return *this;
}

ServerConfig::~ServerConfig() {}

/* ----------------------------------------------------------------------------------- */
/* ServerConfig Setters                                                                */
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
/* ServerConfig Getters                                                                */
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

/* ----------------------------------------------------------------------------------- */
/* ServerConfig Marshalling                                                            */
/* ----------------------------------------------------------------------------------- */
// Unmarshalls a string containing ONLY server config information into a ServerConfig object
void ServerConfig::Unmarshall(std::string str) {
    std::sstream ss(str);

    // vars to unmarshall into:
    std::string host; // required
    uint16_t port; // required
    std::vector<std::string> serverNames; // NOT required
    size_t clientMaxBodySize;
    std::map<uint16_t, std::string> errorPages;
    std::map<std::string, RouteConfig> routes; // AT LEAST ONE required
}

// Returns true if the server config is valid, false otherwise
bool ServerConfig::isValid() const {
    // Required server fields
    if (_host.empty() || _port == 0 || _routes.empty())
        return false;
        
    // Required location fields
    for (std::map<std::string, RouteConfig>::const_iterator it = _routes.begin(); 
         it != _routes.end(); ++it) {
        if (it->second.root.empty() || it->second.allowedMethods.empty())
            return false;
    }
    
    return true;
}

/* ----------------------------------------------------------------------------------- */
/* RouteConfig Constructor & Destructor                                                */
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

RouteConfig::RouteConfig(const RouteConfig& other) {
    *this = other;
}

RouteConfig& RouteConfig::operator=(const RouteConfig& other) {
    if (this != &other) {
        path = other.path;
        root = other.root;
        index = other.index;
        allowedMethods = other.allowedMethods;
        autoindex = other.autoindex;
        redirect = other.redirect;
        cgiExtensions = other.cgiExtensions;
        uploadDir = other.uploadDir;
        clientMaxBodySize = other.clientMaxBodySize;
    }
    return *this;
}

RouteConfig::~RouteConfig() {}

/* ----------------------------------------------------------------------------------- */
/* RouteConfig Unmarshalling                                                           */
/* ----------------------------------------------------------------------------------- */
// Unmarshalls a string containing ONLY route config information into a RouteConfig object
void RouteConfig::Unmarshall(std::string str) {
    std::sstream ss(str);

    // vars to unmarshall into:
    std::string path; // required
    std::string root; // required
    std::vector<std::string> index; // required
    std::set<std::string> allowedMethods; // required
    bool autoindex; // required
    std::string redirect; // required
    std::map<std::string, std::string> cgiExtensions; // required
    std::string uploadDir; // required
    size_t clientMaxBodySize; // required
}

// Returns true if the server config is valid, false otherwise
bool RouteConfig::isValid() const {
    // TODO: implement this
}
