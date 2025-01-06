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
	if (listen(m_socket, SOMAXCONN) < 0) {
		std::cerr << "Listen failed" << std::endl;
		return 1;
	}

//	add socket to pollfds
	pollfd pfd;
	pfd.fd = _socket;
	pfd.events = POLLIN;
	_pollFds.push_back(pfd);

//	start accepting connections
	while (true) {
		int pollCount = poll(m_fds.data(), m_fds.size(), -1);
		if (pollCount < 0) {
			std::cerr << "Poll failed" << std::endl;
			return 1;
		}

		for (size_t i = 0; i < m_fds.size(); ++i) {
			if (m_fds[i].revents & POLLIN) {
				if (m_fds[i].fd == m_socket) {
					int clientSocket = accept(m_socket, nullptr, nullptr);
					if (clientSocket >= 0) {
						setNonBlocking(clientSocket);
						pollfd clientFd;
						clientFd.fd = clientSocket;
						clientFd.events = POLLIN;
						m_fds.push_back(clientFd);
					}
				} else {
					handleClient(m_fds[i].fd);
					close(m_fds[i].fd);
					m_fds.erase(m_fds.begin() + i);
					--i;
				}
			}
		}
	}

	return 0;
}