//
// Created by Leon David Zipp on 1/6/25.
//

#pragma once

#include <vector>
#include "TCPSubServer.hpp"

// holds all subserver instances
class TCPServer {
	public:
		TCPServer(std::vector<TCPSubServer&> servers);
		TCPServer(const TCPServer& other);
		TCPServer& operator=(const TCPServer& other);
		~TCPServer();

//		starts all servers
		int Setup();
		int Start();
		TCPSubServer& TCPServer::GetResponsibleServer(int clientSocket);

	private:
//		first server is fallback server
		std::vector<TCPSubServer&> _servers;
//		1 pollfd per server
		std::vector<pollfd> _pollFds;
//		1 socket per client
		std::map<int, TCPSubServer&> _socketToServer;
};

