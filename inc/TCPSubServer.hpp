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

//a single server instance
class TCPSubServer {
	public:
		// Constructor & Destructor
		TCPSubServer(ServerConfig& config);
		TCPSubServer(const TCPSubServer& other);
		TCPSubServer& operator=(const TCPSubServer& other);
		~TCPSubServer();

		void HandleRequest(int clientSocket);

		ServerConfig& GetConfig() const;
//		int& GetSocket() const;
//		std::vector<pollfd>& GetPollFds() const;

	private:
//		TCPSubServer();

//		variables
		ServerConfig& _config;
		pollfd _pollFd;
};
