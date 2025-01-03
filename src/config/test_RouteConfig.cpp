#include "RouteConfig.hpp"
#include <iostream>

bool _handleRoot(std::string& line, std::string& root);
bool _handleIndex(std::string& line, std::vector<std::string>& index);
bool _handleAllowedMethods(std::string& line, std::set<std::string>& allowedMethods);
bool _handleAutoindex(std::string& line, bool& autoindex);
bool _handleRedirect(std::string& line, std::string& redirect);
bool _handleCgiExtensions(std::string& line, std::map<std::string, std::string>& cgiExtensions);
bool _handleUploadDir(std::string& line, std::string& uploadDir);
bool _handleRouteClientMaxBodySize(std::string& line, size_t& clientMaxBodySize);

bool test_handleRoot() {
	try {
		std::string valid = "root /var/www/html;";
		std::string invalid = "root /var/www/html; index index.html;";
		std::string root;
		if (!_handleRoot(valid, root)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (root != "/var/www/html") {
			std::cout << "Failed on root value: " << root << std::endl;
			return false;
		}

		if (_handleRoot(invalid, root)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleIndex() {
	try {
		std::string valid = "index index.html;";
		std::string valid2 = "index index.html index.htm;";
		std::string invalid = "index;";
		std::vector<std::string> index;
		if (!_handleIndex(valid, index)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (index.size() != 1 || index[0] != "index.html") {
			std::cout << "Failed on index value: " << index[0] << std::endl;
			return false;
		}

		index.clear();
		if (!_handleIndex(valid2, index)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (index.size() != 2 || index[0] != "index.html" || index[1] != "index.htm") {
			std::cout << "Failed on index value: " << index[0] << std::endl;
			std::cout << "Failed on index value: " << index[1] << std::endl;
			return false;
		}

		if (_handleIndex(invalid, index)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleAllowedMethods() {
	try {
		std::string valid = "allowed_methods GET;";
		std::string valid2 = "allowed_methods GET POST;";
		std::string invalid = "allowed_methods;";
		std::set<std::string> allowedMethods;
		if (!_handleAllowedMethods(valid, allowedMethods)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (allowedMethods.size() != 1 || allowedMethods.find("GET") == allowedMethods.end()) {
			std::cout << "Failed on allowed methods value: " << *allowedMethods.begin() << std::endl;
			return false;
		}

		allowedMethods.clear();
		if (!_handleAllowedMethods(valid2, allowedMethods)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (allowedMethods.size() != 2 || allowedMethods.find("GET") == allowedMethods.end() || allowedMethods.find("POST") == allowedMethods.end()) {
			std::cout << "Failed on allowed methods value: " << *allowedMethods.begin() << std::endl;
			std::cout << "Failed on allowed methods value: " << *allowedMethods.rbegin() << std::endl;
			return false;
		}

		if (_handleAllowedMethods(invalid, allowedMethods)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleAutoindex() {
	try {
		std::string valid = "autoindex on;";
		std::string invalid = "autoindex;";
		bool autoindex;
		if (!_handleAutoindex(valid, autoindex)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (!autoindex) {
			std::cout << "Failed on autoindex value: " << autoindex << std::endl;
			return false;
		}

		if (_handleAutoindex(invalid, autoindex)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleRedirect() {
	try {
		std::string valid = "redirect /redirected;";
		std::string invalid = "redirect;";
		std::string redirect;
		if (!_handleRedirect(valid, redirect)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (redirect != "/redirected") {
			std::cout << "Failed on redirect value: " << redirect << std::endl;
			return false;
		}

		if (_handleRedirect(invalid, redirect)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleCgiExtensions() {
	try {
		std::string valid = "cgi_extension .php /usr/bin/php-cgi;";
		std::string valid2 = "cgi_extension .py /usr/bin/python3;";
		std::string invalid = "cgi_extension;";
		std::string invalid2 = "cgi_extension .php /usr/bin/php-cgi .php7 /usr/bin/php7-cgi;";
		std::map<std::string, std::string> cgiExtensions;
		if (!_handleCgiExtensions(valid, cgiExtensions)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (cgiExtensions.size() != 1 || cgiExtensions.find(".php") == cgiExtensions.end() || cgiExtensions[".php"] != "/usr/bin/php-cgi") {
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.begin()->first << std::endl;
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.begin()->second << std::endl;
			return false;
		}

		if (!_handleCgiExtensions(valid2, cgiExtensions)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (cgiExtensions.size() != 2 || cgiExtensions.find(".php") == cgiExtensions.end() || cgiExtensions.find(".py") == cgiExtensions.end() || cgiExtensions[".php"] != "/usr/bin/php-cgi" || cgiExtensions[".py"] != "/usr/bin/python3") {
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.begin()->first << std::endl;
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.begin()->second << std::endl;
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.rbegin()->first << std::endl;
			std::cout << "Failed on cgi extensions value: " << cgiExtensions.rbegin()->second << std::endl;
			return false;
		}

		cgiExtensions.clear();
		if (_handleCgiExtensions(invalid, cgiExtensions)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}

		cgiExtensions.clear();
		if (_handleCgiExtensions(invalid2, cgiExtensions)) {
			std::cout << "Failed on invalid line: " << invalid2 << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleUploadDir() {
	try {
		std::string valid = "upload_dir /var/www/html/uploads;";
		std::string invalid = "upload_dir;";
		std::string uploadDir;
		if (!_handleUploadDir(valid, uploadDir)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (uploadDir != "/var/www/html/uploads") {
			std::cout << "Failed on upload dir value: " << uploadDir << std::endl;
			return false;
		}

		if (_handleUploadDir(invalid, uploadDir)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

bool test_handleRouteClientMaxBodySize() {
	try {
		std::string valid = "client_max_body_size 1333;";
		std::string valid2 = "client_max_body_size 200000;";
		std::string invalid = "client_max_body_size 1MB;";
		std::string invalid2 = "client_max_body_size abcdef;";
		size_t clientMaxBodySize;
		if (!_handleRouteClientMaxBodySize(valid, clientMaxBodySize)) {
			std::cout << "Failed on valid line: " << valid << std::endl;
			return false;
		}
		if (clientMaxBodySize != 1333) {
			std::cout << "Failed on client max body size value: " << clientMaxBodySize << std::endl;
			return false;
		}

		if (!_handleRouteClientMaxBodySize(valid2, clientMaxBodySize)) {
			std::cout << "Failed on valid line: " << valid2 << std::endl;
			return false;
		}
		if (clientMaxBodySize != 200000) {
			std::cout << "Failed on client max body size value: " << clientMaxBodySize << std::endl;
			return false;
		}

		if (_handleRouteClientMaxBodySize(invalid, clientMaxBodySize)) {
			std::cout << "Failed on invalid line: " << invalid << std::endl;
			return false;
		}
		if (_handleRouteClientMaxBodySize(invalid2, clientMaxBodySize)) {
			std::cout << "Failed on invalid line: " << invalid2 << std::endl;
			return false;
		}
	} catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		return false;
	}

	return true;
}

int main(void) {
	if (!test_handleRoot()) {
		std::cout << "Failed on test_handleRoot" << std::endl;
		return 1;
	}
	if (!test_handleIndex()) {
		std::cout << "Failed on test_handleIndex" << std::endl;
		return 1;
	}
	if (!test_handleAllowedMethods()) {
		std::cout << "Failed on test_handleAllowedMethods" << std::endl;
		return 1;
	}
	if (!test_handleAutoindex()) {
		std::cout << "Failed on test_handleAutoindex" << std::endl;
		return 1;
	}
	if (!test_handleRedirect()) {
		std::cout << "Failed on test_handleRedirect" << std::endl;
		return 1;
	}
	if (!test_handleCgiExtensions()) {
		std::cout << "Failed on test_handleCgiExtensions" << std::endl;
		return 1;
	}
	if (!test_handleUploadDir()) {
		std::cout << "Failed on test_handleUploadDir" << std::endl;
		return 1;
	}
	if (!test_handleRouteClientMaxBodySize()) {
		std::cout << "Failed on test_handleRouteClientMaxBodySize" << std::endl;
		return 1;
	}

	return 0;
}