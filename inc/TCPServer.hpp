//
// Created by Leon David Zipp on 1/4/25.
//

#pragma once

#include <sys/socket.h>
#include "ServerConfig.hpp"

class TCPServer {
	public:
		// Constructor & Destructor
		TCPServer();
		TCPServer(const TCPServer& other);
		TCPServer& operator=(const TCPServer& other);
		~TCPServer();

		int StartServer();

	private:
//		variables
		ServerConfig& _config;
		int _socket;
		std::vector<pollfd> _pollFds;

};
