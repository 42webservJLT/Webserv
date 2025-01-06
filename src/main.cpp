#include "main.hpp"


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: ./webserv [config file]" << std::endl;
		return 1;
	}

	Parser parser(argv[1]);
	std::vector<ServerConfig> serverConfigs = parser.ParseConfig();
	std::vector<TCPServer> servers;
	for (ServerConfig& serverConfig : serverConfigs) {
		TCPServer server(serverConfig);
		server.StartServer();
		servers.push_back(server);
	}

	// [...]

	return 0;
}
