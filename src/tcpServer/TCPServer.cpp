//
// Created by Leon David Zipp on 1/6/25.
//

#include "TCPServer.hpp"

TCPServer::TCPServer(std::vector<TCPSubServer&> servers) : _servers(servers) {}

TCPServer::TCPServer(const TCPServer& other) : _servers(other._servers) {}


TCPServer& TCPServer::operator=(const TCPServer& other) {
	if (this != &other) {
		_servers = other._servers;
	}
	return *this;
}

TCPServer::~TCPServer() {}

// reads http request header to determine which server is responsible
TCPSubServer& TCPServer::GetResponsibleServer(int clientSocket) {
//	read in the request header
//	check which server has the path in its config
//	check if the method is allowed
}

// sets server: creates a socket, binds it, listens on it, and accepts connections indefinitely
int TCPServer::Setup() {
	for (auto server : _servers) {
		// open socket
		int _socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == -1) {
			std::cerr << "Error: Failed to create socket" << std::endl;
			return 1;
		}

		// set socket options
		int flags = fcntl(_socket, F_GETFL, 0);
		fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

		struct addrinfo hints;
		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		// resolve hostname to IP address
		struct addrinfo* res;
		int status = getaddrinfo(server.GetConfig().GetHost().c_str(), std::to_string(server.GetConfig().GetPort()).c_str(), &hints, &res);
		if (status != 0) {
			std::cerr << "Error: Failed to resolve hostname: " << gai_strerror(status) << std::endl;
			close(_socket);
			return 1;
		}

		// bind socket
		if (bind(_socket, res->ai_addr, res->ai_addrlen) == -1) {
			std::cerr << "Error: Failed to bind socket" << std::endl;
			freeaddrinfo(res);
			close(_socket);
			return 1;
		}

		freeaddrinfo(res);

		// start listening
		if (listen(_socket, SOMAXCONN) < 0) {
			std::cerr << "Listen failed" << std::endl;
			close(_socket);
			return 1;
		}

		// initialize pollfd vector
		_pollFds.push_back({ _socket, POLLIN, 0 });
	}

	return 0;
}

int TCPServer::Start() {
	while (True) {
		for (server : _servers) {
			auto _pollFds = server.GetPollFds();
			auto _socket = server.GetSocket();
			int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
			if (pollCount < 0) {
				std::cerr << "Poll failed" << std::endl;
				close(_socket);
				return 1;
			}

			for (auto it = _pollFds.begin(); it != _pollFds.end(); ++it) {
				if (it->revents & POLLIN) {
					if (it->fd == _socket) {
						int clientSocket = accept(_socket, nullptr, nullptr);
						if (clientSocket >= 0) {
							fcntl(clientSocket, F_SETFL, O_NONBLOCK);
							_pollFds.push_back({ clientSocket, POLLIN, 0 });
						}
					} else {
						server.HandleRequest(it->fd);
						close(it->fd);
						it = _pollFds.erase(it);
						if (it == _pollFds.end()) {
							break;
						}
					}
				}
			}
		}
	}
	return 0;
}