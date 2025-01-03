#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "helpers.hpp"
#include "RouteConfig.hpp"

#define DEFAULT_AUTO_INDEX false
#define DEFAULT_CLIENT_MAX_BODY_SIZE 1048576
#define DEFAULT_REDIRECT ""
#define DEFAULT_UPLOAD_DIR ""
// at least for php
#define DEFAULT_CGI_EXTENSIONS std::map<std::string, std::string> { {".php", "/usr/bin/php-cgi"} }

class RouteConfig {
	public:
		RouteConfig();
		RouteConfig(
			const std::string& path_,
			const std::string& root_,
			const std::vector<std::string>& index_,
			const std::set<std::string>& allowedMethods_,
			const bool autoindex_,
			const std::string& redirect_,
			const std::map<std::string, std::string>& cgiExtensions_,
			const std::string& uploadDir_,
			size_t clientMaxBodySize_
		);
		RouteConfig(const RouteConfig& other);
		RouteConfig& operator=(const RouteConfig& other);
		~RouteConfig();

		bool IsValid() const;

		bool Unmarshall(std::string& str);

		std::string                     path;
		std::string                     root;
		std::vector<std::string>        index;
		std::set<std::string>           allowedMethods;
		bool                            autoindex;
		std::string                     redirect;
		std::map<std::string, std::string> cgiExtensions;
		std::string                     uploadDir;
		size_t                          clientMaxBodySize;
};
