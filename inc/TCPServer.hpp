//
// Created by Leon David Zipp on 1/4/25.
//

#pragma once

#include "ServerConfig.hpp"

class TCPServer {
	public:
		// Constructor & Destructor
		TCPServer();
		TCPServer(const TCPServer& other);
		TCPServer& operator=(const TCPServer& other);
		~TCPServer();

	private:
//		TODO: Add members
		void StartServer();

//		variables
		ServerConfig& _config;

};
