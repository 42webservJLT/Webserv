#include "TCPServer.hpp"

/* ----------------------------------------------------------------------------------- */
/* TCPServer Constructor & Destructor                                                  */
/* ----------------------------------------------------------------------------------- */
//TCPServer::TCPServer() : _socket(-1) {}

TCPServer::TCPServer(ServerConfig& config) : _config(config), _socket(-1) {}

TCPServer::TCPServer(const TCPServer& other) : _config(other._config), _socket(other._socket), _pollFds(other._pollFds) {
	*this = other;
}

TCPServer& TCPServer::operator=(const TCPServer& other) {
	if (this != &other) {
		_config = other._config;
		_socket = other._socket;
		_pollFds = other._pollFds;
	}

	return *this;
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
	// open socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
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
	int status = getaddrinfo(_config.GetHost().c_str(), std::to_string(_config.GetPort()).c_str(), &hints, &res);
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

	// initialize fd_set
	fd_set masterSet, readSet;
	FD_ZERO(&masterSet);
	FD_SET(_socket, &masterSet);
	int maxFd = _socket;

	// start accepting connections
	while (true) {
		readSet = masterSet;
		int selectCount = select(maxFd + 1, &readSet, nullptr, nullptr, nullptr);
		if (selectCount < 0) {
			std::cerr << "Select failed" << std::endl;
			close(_socket);
			return 1;
		}

		for (int fd = 0; fd <= maxFd; ++fd) {
			if (FD_ISSET(fd, &readSet)) {
				if (fd == _socket) {
					int clientSocket = accept(_socket, nullptr, nullptr);
					if (clientSocket >= 0) {
						fcntl(clientSocket, F_SETFL, O_NONBLOCK);
						FD_SET(clientSocket, &masterSet);
						if (clientSocket > maxFd) {
							maxFd = clientSocket;
						}
					}
				} else {
					_handleClient(fd);
					close(fd);
					FD_CLR(fd, &masterSet);
				}
			}
		}
	}

	close(_socket);
	return 0;
}

// handles a client connection
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