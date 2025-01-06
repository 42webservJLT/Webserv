#include "TCPServer.hpp"

/* ----------------------------------------------------------------------------------- */
/* TCPServer Constructor & Destructor                                                  */
/* ----------------------------------------------------------------------------------- */
TCPServer::TCPServer() _socket(-1) {}

TCPServer::TCPServer(const TCPServer& other) {
	*this = other;
}

TCPServer& TCPServer::operator=(const TCPServer& other) {
	if (this != &other) {
		_config = other._config;
		_socket = other._socket;
		_pollFds = other._pollFds;
	}
}

TCPServer::~TCPServer() {
	if (_socket != -1) {
		close(_socket);
	}
}

/* ----------------------------------------------------------------------------------- */
/* TCPServer Start                                                                     */
/* ----------------------------------------------------------------------------------- */
// starts the server: creates a socket, binds it, listens on it, and accepts connections indefinitely
int TCPServer::StartServer() {
//	open socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		std::cerr << "Error: Failed to create socket" << std::endl;
		return;
	}

//	set socket options
	int flags = fcntl(_socket, F_GETFL, 0);
	fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(_config.GetPort());

//	bind socket
	if (bind(_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cerr << "Error: Failed to bind socket" << std::endl;
		close(_socket);
		return;
	}

//	start listening
	if (listen(_socket, SOMAXCONN) < 0) {
		std::cerr << "Listen failed" << std::endl;
		close(_socket);
		return 1;
	}

//	add socket to pollFds
	pollfd pfd;
	pfd.fd = _socket;
	pfd.events = POLLIN;
	_pollFds.push_back(pfd);

//	start accepting connections
	while (true) {
		int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
		if (pollCount < 0) {
			std::cerr << "Poll failed" << std::endl;
			close(_socket);
			return 1;
		}

		for (size_t i = 0; i < _pollFds.size(); ++i) {
			if (_pollFds[i].revents & POLLIN) {
				if (_pollFds[i].fd == _socket) {
					int clientSocket = accept(_socket, nullptr, nullptr);
					if (clientSocket >= 0) {
						setNonBlocking(clientSocket);
						pollfd clientFd;
						clientFd.fd = clientSocket;
						clientFd.events = POLLIN;
						_pollFds.push_back(clientFd);
					}
				} else {
					handleClient(_pollFds[i].fd);
					close(_pollFds[i].fd);
					_pollFds.erase(_pollFds.begin() + i);
					--i;
				}
			}
		}
	}

	close(_socket);
	return 0;
}

void TCPServer::_handleClient(int clientSocket) {
	char buffer[_config.GetClientMaxBodySize()];
	std::memset(buffer, 0, sizeof(buffer));
	ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
	if (bytesRead > 0) {
//		TODO: handle request
	} else if (bytesRead == 0) {
		std::cout << "Client disconnected" << std::endl;
	} else {
		std::cerr << "Error: Failed to read from client" << std::endl;
	}
}