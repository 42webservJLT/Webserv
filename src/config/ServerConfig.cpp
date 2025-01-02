#include "ServerConfig.hpp"

bool _lineValid(std::string& line);
bool _handleHost(std::string& line, std::string& host);
bool _handlePort(std::string& line, uint16_t& port);
bool _handleServerName(std::string& line, std::vector<std::string>& serverNames);
bool _handleErrorPage(std::string& line, std::map<uint16_t, std::string>& errorPages);
bool _handleLocation(std::string& line, std::map<std::string, RouteConfig>& routes);

/* ----------------------------------------------------------------------------------- */
/* ServerConfig Constructor & Destructor                                               */
/* ----------------------------------------------------------------------------------- */
ServerConfig::ServerConfig() {}

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
bool ServerConfig::Unmarshall(std::string& str) {
	// vars to unmarshall into:
	std::string host = DEFAULT_HOST_NAME; // required, ONLY ONE
	uint16_t port = DEFAULT_PORT; // required, ONLY ONE
	std::vector<std::string> serverNames; // NOT required, default to empty: ""
	size_t clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE; // NOT required, default to 1MB
	std::map<uint16_t, std::string> errorPages; // NOT required, default to empty
	std::map<std::string, RouteConfig> routes; // AT LEAST ONE required

	// unmarshall
	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line)) {
		if (line.empty()) {
			continue;
		} else if (!_lineValid(line)) {
			return false;
		}
		
		if (line.size() >= 5 && line.substr(0, 5) == "host ") {
			if (!_handleHost(line, host)) {
				return false;
			}
		} else if (line.size() >= 5 && line.substr(0, 5) == "port ") {
			if (!_handlePort(line, port)) {
				return false;
			}
		} else if (line.size() >= 12 && line.substr(0, 12) == "server_name ") {
			if (!_handleServerName(line, serverNames)) {
				return false;
			}
		} else if (line.size() >= 20 && line.substr(0, 20) == "client_max_body_size ") {
			clientMaxBodySize = std::stoi(line.substr(20));
		} else if (line.size() >= 11 && line.substr(0, 11) == "error_page ") {
			if (!_handleErrorPage(line, errorPages)) {
				return false;
			}
		} else if (line.size() >= 9 && line.substr(0, 9) == "location ") {
			if (!_handleLocation(line, routes)) {
				return false;
			}
		} else {
			return false;
		}
	}

	// validate all required fields are present
	if (host == DEFAULT_HOST_NAME
		|| port == DEFAULT_PORT
		|| routes.empty()
	) {
		return false;
	} else if (routes.empty()) {
		return false;
	}

	// set the unmarshalled values
	_host = host;
	_port = port;
	_serverNames = serverNames;
	_clientMaxBodySize = clientMaxBodySize;
	_errorPages = errorPages;
	_routes = routes;

	return true;
}

// checks if a line is valid to begin with
bool _lineValid(std::string& line) {
	// a line is valid if ...
	// 1. it is the server line
	if (line.size() >= 6 && line.substr(0, 6) == "server") {
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> tokens;
		while (std::getline(ss, token, ' ')) {
			tokens.push_back(token);
		}

		if (tokens.size() != 2) {
			return false;
		} else if (tokens[1] != "{") {
			return true;
		} else {
			return false;
		}
	}
	// 2. it is the last line of the server block
	else if (line.size() == 1 && line[0] == '}') {
		return true;
	}
	// 3. it ends with ';'
	else if (line.back() == ';') {
		return true;
	} else {
		return false;
	}
}

bool _handleHost(std::string& line, std::string& host) {
	// split line containing host by  space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	host = tokens[1];
	return true;
}

bool _handlePort(std::string& line, uint16_t& port) {
	// split line containing port by  space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	port = std::stoi(tokens[1]);
	return true;
}

bool _handleServerName(std::string& line, std::vector<std::string>& serverNames) {
	// split line containing server_name by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() == 1) {
		return false;
	}
	serverNames.push_back(tokens[1]);
	return true;
}

bool _handleErrorPage(std::string& line, std::map<uint16_t, std::string>& errorPages) {
	// split line containing error_page by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 3) {
		return false;
	}
	errorPages[std::stoi(tokens[1])] = tokens[2];
	return true;
}

bool _handleLocation(std::string& line, std::map<std::string, RouteConfig>& routes) {
	// split line containing location by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	RouteConfig route;
	route.root = tokens[1];
	routes[tokens[1]] = route;
	return true;
}
