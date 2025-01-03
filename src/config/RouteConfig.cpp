#include "RouteConfig.hpp"

bool _handleRoot(std::string& line, std::string& root);
bool _handleIndex(std::string& line, std::vector<std::string>& index);
bool _handleAllowedMethods(std::string& line, std::set<std::string>& allowedMethods);
bool _handleAutoindex(std::string& line, bool& autoindex);
bool _handleRedirect(std::string& line, std::string& redirect);
bool _handleCgiExtensions(std::string& line, std::map<std::string, std::string>& cgiExtensions);
bool _handleUploadDir(std::string& line, std::string& uploadDir);
bool _handleRouteClientMaxBodySize(std::string& line, size_t& clientMaxBodySize);

/* ----------------------------------------------------------------------------------- */
/* RouteConfig Constructor & Destructor                                                */
/* ----------------------------------------------------------------------------------- */
RouteConfig::RouteConfig() {}

RouteConfig::RouteConfig(
	const std::string& path_,
	const std::string& root_,
	const std::vector<std::string>& index_,
	const std::set<std::string>& allowedMethods_,
	const bool autoindex_,
	const std::string& redirect_,
	const std::map<std::string, std::string>& cgiExtensions_,
	const std::string& uploadDir_,
	size_t clientMaxBodySize_
) {
	path = path_;
	root = root_;
	index = index_;
	allowedMethods = allowedMethods_;
	autoindex = autoindex_;
	redirect = redirect_;
	cgiExtensions = cgiExtensions_;
	uploadDir = uploadDir_;
	clientMaxBodySize = clientMaxBodySize_;
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
bool RouteConfig::Unmarshall(std::string& str) {
	// vars to unmarshall into:
	std::string path; // required
	std::string root; // required
	std::vector<std::string> index; // required
	std::set<std::string> allowedMethods; // required
	bool autoindex = DEFAULT_AUTO_INDEX; // optional
	std::string redirect = DEFAULT_REDIRECT; // optional
	std::map<std::string, std::string> cgiExtensions = DEFAULT_CGI_EXTENSIONS; // optional
	std::string uploadDir = DEFAULT_UPLOAD_DIR; // optional
	size_t clientMaxBodySize = DEFAULT_CLIENT_MAX_BODY_SIZE; // optional

	std::stringstream ss(str);
	std::string line;
	while (std::getline(ss, line)) {
		if (line.empty()) {
			continue;
		} else if (line == "}") {
			break;
		}

		if (line.size() >= 5 && line.substr(0, 5) == "root ") {
			if (!_handleRoot(line, root)) {
				return false;
			}
		} else if (line.size() >= 6 && line.substr(0, 6) == "index ") {
			if (!_handleIndex(line, index)) {
				return false;
			}
		} else if (line.size() >= 15 && line.substr(0, 15) == "allowed_methods ") {
			if (!_handleAllowedMethods(line, allowedMethods)) {
				return false;
			}
		} else if (line.size() >= 10 && line.substr(0, 10) == "autoindex ") {
			if (!_handleAutoindex(line, autoindex)) {
				return false;
			}
		} else if (line.size() >= 9 && line.substr(0, 9) == "redirect ") {
			if (!_handleRedirect(line, redirect)) {
				return false;
			}
		} else if (line.size() >= 13 && line.substr(0, 13) == "cgi_extension ") {
			if (!_handleCgiExtensions(line, cgiExtensions)) {
				return false;
			}
		} else if (line.size() >= 11 && line.substr(0, 11) == "upload_dir ") {
			if (!_handleUploadDir(line, uploadDir)) {
				return false;
			}
		} else if (line.size() >= 19 && line.substr(0, 19) == "client_max_body_size ") {
			if (!_handleRouteClientMaxBodySize(line, clientMaxBodySize)) {
				return false;
			}
		} else {
			return false;
		}
	}
}

// handles the root line
bool _handleRoot(std::string& line, std::string& root) {
	// split line containing root by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	root = tokens[1];
	return true;
}

// handles the index line
bool _handleIndex(std::string& line, std::vector<std::string>& index) {
	// split line containing index by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() < 2) {
		return false;
	}
	for (size_t i = 1; i < tokens.size(); i++) {
		index.push_back(tokens[i]);
	}
	return true;
}

// handles the allowed_methods line
bool _handleAllowedMethods(std::string& line, std::set<std::string>& allowedMethods) {
	// split line containing allowed_methods by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() < 2) {
		return false;
	}
	for (size_t i = 1; i < tokens.size(); i++) {
		allowedMethods.insert(tokens[i]);
	}
	return true;
}

// handles the autoindex line
bool _handleAutoindex(std::string& line, bool& autoindex) {
	// split line containing autoindex by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	if (tokens[1] == "on") {
		autoindex = true;
	} else if (tokens[1] == "off") {
		autoindex = false;
	} else {
		return false;
	}
	return true;
}

// handles the redirect line
bool _handleRedirect(std::string& line, std::string& redirect) {
	// split line containing redirect by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	redirect = tokens[1];
	return true;
}

// handles the cgi_extensions line
bool _handleCgiExtensions(std::string& line, std::map<std::string, std::string>& cgiExtensions) {
	// split line containing cgi_extensions by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() < 3 || tokens.size() % 2 != 1) {
		return false;
	}
	for (size_t i = 1; i < tokens.size(); i += 2) {
		cgiExtensions[tokens[i]] = tokens[i + 1];
	}
	return true;
}

// handles the upload_dir line
bool _handleUploadDir(std::string& line, std::string& uploadDir) {
	// split line containing upload_dir by space chars
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(iss, token, ' ')) {
		tokens.push_back(token);
	}
	if (tokens.size() != 2) {
		return false;
	}
	uploadDir = tokens[1];
	return true;
}

// handles the client_max_body_size line
bool _handleRouteClientMaxBodySize(std::string& line, size_t& clientMaxBodySize) {
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
		clientMaxBodySize = std::stoi(tokens[1]);
	} catch (std::exception& e) {
		return false;
	}

	return true;
}
