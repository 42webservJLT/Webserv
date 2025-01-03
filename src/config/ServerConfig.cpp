#include "ServerConfig.hpp"

bool _lineValid(std::string& line);
bool _handleHost(std::string& line, std::string& host);
bool _handlePort(std::string& line, uint16_t& port);
bool _handleServerName(std::string& line, std::vector<std::string>& serverNames);
bool _handleErrorPage(std::string& line, std::map<HttpStatus, std::string>& errorPages);
bool _handleClientMaxBodySize(std::string& line, size_t& clientMaxBodySize);
bool _handleLocation(std::string& line, std::ifstream& file, std::map<std::string, RouteConfig>& routes);
bool _readRouteBlock(const std::string& firstLine, std::ifstream& file, std::string& routeBlock);

/* ----------------------------------------------------------------------------------- */
/* ServerConfig Constructor & Destructor                                               */
/* ----------------------------------------------------------------------------------- */
ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(
	const std::string& host,
	uint16_t port,
	const std::vector<std::string>& serverNames,
	size_t clientMaxBodySize,
	std::map<HttpStatus, std::string> errorPages,
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

void ServerConfig::AddErrorPage(HttpStatus code, const std::string& path) {
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

const std::map<HttpStatus, std::string>& ServerConfig::GetErrorPages() const {
    return _errorPages;
}

const std::string& ServerConfig::GetErrorPage(HttpStatus code) const {
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
bool ServerConfig::Unmarshall(std::string& str, std::ifstream& file) {
	// vars to unmarshall into:
	std::string host = DEFAULT_HOST_NAME; // required, ONLY ONE
	uint16_t port = DEFAULT_PORT; // required, ONLY ONE
	std::vector<std::string> serverNames; // NOT required, default to empty: ""
	size_t clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE; // NOT required, default to 1MB
	std::map<HttpStatus, std::string> errorPages; // NOT required, default to empty
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
			if (!_handleClientMaxBodySize(line, clientMaxBodySize)) {
				return false;
			}
		} else if (line.size() >= 11 && line.substr(0, 11) == "error_page ") {
			if (!_handleErrorPage(line, errorPages)) {
				return false;
			}
		} else if (line.size() >= 9 && line.substr(0, 9) == "location ") {
			if (!_handleLocation(line, file, routes)) {
				return false;
			}
		} else {
			return false;
		}
	}

	// fill & validate all empty fields with default values
	if (host == DEFAULT_HOST_NAME
		|| port == DEFAULT_PORT
		|| routes.empty()
	) {
		return false;
	} else if (serverNames.empty()) {
		serverNames.push_back(DEFAULT_SERVER_NAME);
	}

	if (errorPages.empty()) {
		errorPages[HttpStatus::BAD_REQUEST] = DEFAULT_BAD_REQUEST_PATH;
		errorPages[HttpStatus::UNAUTHORIZED] = DEFAULT_UNAUTHORIZED_PATH;
		errorPages[HttpStatus::FORBIDDEN] = DEFAULT_FORBIDDEN_PATH;
		errorPages[HttpStatus::NOT_FOUND] = DEFAULT_NOT_FOUND_PATH;
		errorPages[HttpStatus::METHOD_NOT_ALLOWED] = DEFAULT_METHOD_NOT_ALLOWED_PATH;
		errorPages[HttpStatus::PAYLOAD_TOO_LARGE] = DEFAULT_PAYLOAD_TOO_LARGE_PATH;
		errorPages[HttpStatus::URI_TOO_LONG] = DEFAULT_URI_TOO_LONG_PATH;
		errorPages[HttpStatus::INTERNAL_SERVER_ERROR] = DEFAULT_INTERNAL_SERVER_ERROR_PATH;
		errorPages[HttpStatus::NOT_IMPLEMENTED] = DEFAULT_NOT_IMPLEMENTED_PATH;
		errorPages[HttpStatus::BAD_GATEWAY] = DEFAULT_BAD_GATEWAY_PATH;
		errorPages[HttpStatus::SERVICE_UNAVAILABLE] = DEFAULT_SERVICE_UNAVAILABLE_PATH;
		errorPages[HttpStatus::GATEWAY_TIMEOUT] = DEFAULT_GATEWAY_TIMEOUT_PATH;
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
	if (line.size() >= 7 && line.substr(0, 7) == "server ") {
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> tokens;
		while (std::getline(ss, token, ' ')) {
			tokens.push_back(token);
		}

		if (tokens.size() != 2) {
			return false;
		} else if (tokens[1] == "{") {
			return true;
		} else {
			return false;
		}
	}
	// 2. it is the last line of the server block
	else if (line.size() == 1 && line[0] == '}') {
		return true;
	} else if (std::count(line.begin(), line.end(), ';') > 1) {
		return false;
	}
	// 3. it ends with ';' & does not include '{' or '}'
	else if (line.size() > 1 && line.back() == ';'
		&& line.find('{') == std::string::npos && line.find('}') == std::string::npos) {
		return true;
	} else {
		return false;
	}
}

// gets the host name
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
	host = tokens[1].substr(0, tokens[1].size() - 1);
	return true;
}

// gets the port number
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

	try {
		port = std::stoi(tokens[1].substr(0, tokens[1].size() - 1));
	} catch (std::exception& e) {
		return false;
	}

	return true;
}

// gets the server names
bool _handleServerName(std::string& line, std::vector<std::string>& serverNames) {
	// split line containing server_name by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	// std::cout << "---------\n";
	while (std::getline(iss, token, ' ')) {
		// std::cout << "token: " << token << std::endl;
		tokens.push_back(token);
	}
	if (tokens.size() == 1) {
		return false;
	}

	for (size_t i = 1; i < tokens.size(); i++) {
		if (tokens[i].back() == ';') {
			serverNames.push_back(tokens[i].substr(0, tokens[i].size() - 1));
		} else {
			serverNames.push_back(tokens[i]);
		}
	}

	return true;
}

bool _handleErrorPage(std::string& line, std::map<HttpStatus, std::string>& errorPages) {
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
	
	// handle error code
	int c;
	try {
		c = std::stoi(tokens[1]);
	} catch (std::exception& e) {
		return false;
	}

	// assign to enum
	if (tokens[2].back() == ';') {
		tokens[2] = tokens[2].substr(0, tokens[2].size() - 1);
	}
	switch (c) {
		case 400:
			errorPages[HttpStatus::BAD_REQUEST] = tokens[2];
			break;
		case 401:
			errorPages[HttpStatus::UNAUTHORIZED] = tokens[2];
			break;
		case 403:
			errorPages[HttpStatus::FORBIDDEN] = tokens[2];
			break;
		case 404:
			errorPages[HttpStatus::NOT_FOUND] = tokens[2];
			break;
		case 405:
			errorPages[HttpStatus::METHOD_NOT_ALLOWED] = tokens[2];
			break;
		case 413:
			errorPages[HttpStatus::PAYLOAD_TOO_LARGE] = tokens[2];
			break;
		case 414:
			errorPages[HttpStatus::URI_TOO_LONG] = tokens[2];
			break;
		case 500:
			errorPages[HttpStatus::INTERNAL_SERVER_ERROR] = tokens[2];
			break;
		case 501:
			errorPages[HttpStatus::NOT_IMPLEMENTED] = tokens[2];
			break;
		case 502:
			errorPages[HttpStatus::BAD_GATEWAY] = tokens[2];
			break;
		case 503:
			errorPages[HttpStatus::SERVICE_UNAVAILABLE] = tokens[2];
			break;
		case 504:
			errorPages[HttpStatus::GATEWAY_TIMEOUT] = tokens[2];
			break;
		default:
			return false;
	}

	return true;
}

bool _handleClientMaxBodySize(std::string& line, size_t& clientMaxBodySize) {
	// split line containing client_max_body_size by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}

	try {
		tokens[1] = tokens[1].substr(0, tokens[1].size() - 1);
		if (!std::all_of(tokens[1].begin(), tokens[1].end(), ::isdigit)) {
			return false;
		}
		clientMaxBodySize = std::stoi(tokens[1]);
	} catch (std::exception& e) {
		return false;
	}

	return true;
}

bool _handleLocation(std::string& line, std::ifstream& file, std::map<std::string, RouteConfig>& routes) {
	// split line containing location by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 3 || tokens[2] != "{") {
		return false;
	}

	std::string routeBlock;
	if (!_readRouteBlock(line, file, routeBlock)) {
		return false;
	}

	RouteConfig route;
	if (!route.Unmarshall(line)) {
		return false;
	}

	return true;
}

// reads a server block from the file
bool _readRouteBlock(const std::string& firstLine, std::ifstream& file, std::string& routeBlock) {
	size_t leftBrackets = 0;
	size_t rightBrackets = 0;

	if (firstLine[firstLine.size() - 1] == '{') {
		leftBrackets++;
	}

	routeBlock = firstLine + "\n";
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("{") != std::string::npos) {
			++leftBrackets;
			if (leftBrackets > 1) {
				return false;
			}
		} else if (line.find("}") != std::string::npos) {
			++rightBrackets;
			if (rightBrackets > 1) {
				return false;
			}
		}

		if (leftBrackets == rightBrackets) {
			routeBlock += line;
			break;
		} else {
			routeBlock += line + "\n";
		}
	}
	return true;
}
