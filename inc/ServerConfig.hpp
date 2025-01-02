#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "RouteConfig.hpp"

#define DEFAULT_HOST_NAME ""
#define DEFAULT_PORT 0
#define DEFAULT_SERVER_NAME ""
#define DEFAULT_CLIENT_MAX_BODY_SIZE 1048576
#define DEFAULT_ERROR_PAGE_PATH "" // TODO: change

class ServerConfig {
	public:
		// Constructor & Destructor
		ServerConfig();
		ServerConfig(
			const std::string& host,
			uint16_t port,
			const std::vector<std::string>& serverNames,
			size_t clientMaxBodySize,
			std::map<uint16_t, std::string> errorPages,
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
		void AddErrorPage(uint16_t code, const std::string& path);
		void AddRoute(const std::string& path, const RouteConfig& config);

		// Getters
		const std::string& GetHost() const;
		uint16_t GetPort() const;
		const std::vector<std::string>& GetServerNames() const;
		size_t GetClientMaxBodySize() const;
		const std::map<uint16_t, std::string>& GetErrorPages() const;
		const std::string& GetErrorPage(uint16_t code) const;
		const std::map<std::string, RouteConfig>& GetRoutes() const;
		const RouteConfig& GetRoute(const std::string& path) const;

		// Unmarshalling
		bool Unmarshall(std::string& str);

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
		std::map<uint16_t, std::string>     _errorPages;
		// routes: the routes of the server
		std::map<std::string, RouteConfig>  _routes;
};