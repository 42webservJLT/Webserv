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
/* TCPServer Getters                                                                  */
/* ----------------------------------------------------------------------------------- */
ServerConfig& TCPServer::GetConfig() const {
	return _config;
}

int& TCPServer::GetSocket() const {
	return _socket;
}

std::vector<pollfd>& TCPServer::GetPollFds() const {
	return _pollFds;
}

/* ----------------------------------------------------------------------------------- */
/* TCPServer Start                                                                     */
/* ----------------------------------------------------------------------------------- */
// starts the server: creates a socket, binds it, listens on it, and accepts connections indefinitely
int TCPServer::Setup() {
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

	// initialize pollfd vector
	_pollFds.push_back({ _socket, POLLIN, 0 });

//	announce server start
	std::cout << "Server started on " << _config.GetHost() << ":" << _config.GetPort() << std::endl;

	// start accepting connections
//	while (true) {
//		int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
//		if (pollCount < 0) {
//			std::cerr << "Poll failed" << std::endl;
//			close(_socket);
//			return 1;
//		}
//
//		for (auto it = _pollFds.begin(); it != _pollFds.end(); ++it) {
//			if (it->revents & POLLIN) {
//				if (it->fd == _socket) {
//					int clientSocket = accept(_socket, nullptr, nullptr);
//					if (clientSocket >= 0) {
//						fcntl(clientSocket, F_SETFL, O_NONBLOCK);
//						_pollFds.push_back({ clientSocket, POLLIN, 0 });
//					}
//				} else {
//					_handleClient(it->fd);
//					close(it->fd);
//					it = _pollFds.erase(it);
//					if (it == _pollFds.end()) {
//						break;
//					}
//				}
//			}
//		}
//	}

	close(_socket);
	return 0;
}

// handles a client connection
void TCPServer::_handleClient(int clientSocket) {
	char buffer[_config.GetClientMaxBodySize()];
	std::memset(buffer, 0, sizeof(buffer));
	ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
	if (bytesRead > 0) {
		std::cout << "Received: " << buffer << std::endl;
//		std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
//		write(clientSocket, response.c_str(), response.size());
	} else if (bytesRead == 0) {
		std::cout << "Client disconnected" << std::endl;
	} else {
		std::cerr << "Error: Failed to read from client" << std::endl;
	}
}