#include "RouteConfig.hpp"

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
	bool autoindex; // required
	std::string redirect; // required
	std::map<std::string, std::string> cgiExtensions; // required
	std::string uploadDir; // required
	size_t clientMaxBodySize; // required

	std::stringstream ss(str);
}

// Returns true if the server config is valid, false otherwise
bool RouteConfig::IsValid() const {
	// TODO: implement this
}
