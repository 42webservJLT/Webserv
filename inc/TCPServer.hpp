//
// Created by Leon David Zipp on 1/4/25.
//

#pragma once

#include <sys/socket.h>
#include <poll.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "ServerConfig.hpp"

class TCPServer {
	public:
		// Constructor & Destructor
		TCPServer(ServerConfig& config);
		TCPServer(const TCPServer& other);
		TCPServer& operator=(const TCPServer& other);
		~TCPServer();

		int Setup();
		ServerConfig& GetConfig() const;
		int& GetSocket() const;
		std::vector<pollfd>& GetPollFds() const;

	private:
//		TCPServer();

//		variables
		ServerConfig& _config;
		int _socket;
		std::vector<pollfd> _pollFds;

		void _handleClient(int clientSocket);
};
