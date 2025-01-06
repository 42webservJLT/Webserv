#include "main.hpp"


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "usage: ./webserv [config file]" << std::endl;
		return 1;
	}

	Parser parser(argv[1]);
	std::vector<ServerConfig> serverConfigs = parser.ParseConfig();
	std::vector<TCPSubServer> servers;
	for (ServerConfig& serverConfig : serverConfigs) {
		TCPSubServer server(serverConfig);
		server.Setup()
		servers.push_back(server);
	}

	// [...]

	return 0;
}
