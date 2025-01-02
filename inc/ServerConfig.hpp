#pragma once

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "HTTPStatus.hpp"
#include "RouteConfig.hpp"

// default values
#define DEFAULT_HOST_NAME ""
#define DEFAULT_PORT 0
#define DEFAULT_SERVER_NAME ""
#define DEFAULT_CLIENT_MAX_BODY_SIZE 1048576

// error paths
#define DEFAULT_BAD_REQUEST_PATH "/400.html"
#define DEFAULT_UNAUTHORIZED_PATH "/401.html"
#define DEFAULT_FORBIDDEN_PATH "/403.html"
#define DEFAULT_NOT_FOUND_PATH "/404.html"
#define DEFAULT_METHOD_NOT_ALLOWED_PATH "/405.html"
#define DEFAULT_PAYLOAD_TOO_LARGE_PATH "/413.html"
#define DEFAULT_URI_TOO_LONG_PATH "/414.html"
#define DEFAULT_INTERNAL_SERVER_ERROR_PATH "/500.html"
#define DEFAULT_NOT_IMPLEMENTED_PATH "/501.html"
#define DEFAULT_BAD_GATEWAY_PATH "/502.html"
#define DEFAULT_SERVICE_UNAVAILABLE_PATH "/503.html"
#define DEFAULT_GATEWAY_TIMEOUT_PATH "/504.html"

// holds the config of a single server
class ServerConfig {
	public:
		// Constructor & Destructor
		ServerConfig();
		ServerConfig(
			const std::string& host,
			uint16_t port,
			const std::vector<std::string>& serverNames,
			size_t clientMaxBodySize,
			std::map<HttpStatus, std::string> errorPages,
			std::map<std::string, RouteConfig> routes
		);
		ServerConfig(const ServerConfig& other);
		ServerConfig& operator=(const ServerConfig& other);
		~ServerConfig();

		// Setters (maybe remove, can be done in constructor)
		void SetHost(const std::string& host);
		void SetPort(uint16_t port);
		void AddServerName(const std::string& name);
		void SetClientMaxBodySize(size_t size);
		void AddErrorPage(HttpStatus code, const std::string& path);
		void AddRoute(const std::string& path, const RouteConfig& config);

		// Getters
		const std::string& GetHost() const;
		uint16_t GetPort() const;
		const std::vector<std::string>& GetServerNames() const;
		size_t GetClientMaxBodySize() const;
		const std::map<HttpStatus, std::string>& GetErrorPages() const;
		const std::string& GetErrorPage(HttpStatus code) const;
		const std::map<std::string, RouteConfig>& GetRoutes() const;
		const RouteConfig& GetRoute(const std::string& path) const;

		// Unmarshalling
		bool Unmarshall(std::string& str, std::ifstream& file);

//		Validation
		bool IsValid() const;

	private:
		// host: the host of the server
		std::string                         _host;
		// port: the port of the server
		uint16_t                            _port;
		// serverNames: the server names of the server
		std::vector<std::string>            _serverNames;
		// clientMaxBodySize: the maximum body size of the client
		size_t                              _clientMaxBodySize;
		// errorPages: the error pages of the server
		std::map<HttpStatus, std::string>     _errorPages;
		// routes: the routes of the server
		std::map<std::string, RouteConfig>  _routes;
};