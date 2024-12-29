#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

class RouteConfig {
    public:
        RouteConfig(
            const std::string& path,
            const std::string& root,
            const std::vector<std::string>& index,
            const std::set<std::string>& allowedMethods,
            bool autoindex,
            const std::string& redirect,
            const std::map<std::string, std::string>& cgiExtensions,
            const std::string& uploadDir,
            size_t clientMaxBodySize
        );
        ~RouteConfig();

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

class ServerConfig {
    public:
        // Constructor & Destructor
        ServerConfig(
            const std::string& host,
            uint16_t port,
            const std::vector<std::string>& serverNames,
            size_t clientMaxBodySize
            std::map<uint16_t, std::string> errorPages,
            std::map<std::string, RouteConfig> routes
        );
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
        const std::string& GetRoute(const std::string& path) const;

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