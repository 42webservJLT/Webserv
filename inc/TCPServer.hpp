//
// Created by Leon David Zipp on 1/6/25.
//

#pragma once

#include <vector>
#include "TCPSubServer.hpp"

//a manager for multiple (sub)servers
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
		std::vector<TCPSubServer&> _servers;
		std::vector<pollfd> _pollFds;
		std::vector<int> _sockets;
};

