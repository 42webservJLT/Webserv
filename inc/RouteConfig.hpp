#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "RouteConfig.hpp"

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

		void Unmarshall(std::string& str);

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
