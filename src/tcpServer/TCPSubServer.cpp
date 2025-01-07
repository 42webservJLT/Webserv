#include "TCPSubServer.hpp"

/* ----------------------------------------------------------------------------------- */
/* TCPSubServer Constructor & Destructor                                                  */
/* ----------------------------------------------------------------------------------- */
//TCPSubServer::TCPSubServer() : _socket(-1) {}

TCPSubServer::TCPSubServer(ServerConfig& config) : _config(config) {}

TCPSubServer::TCPSubServer(const TCPSubServer& other) : _config(other._config) {
	*this = other;
}

TCPSubServer& TCPSubServer::operator=(const TCPSubServer& other) {
	if (this != &other) {
		_config = other._config;
	}

	return *this;
}

TCPSubServer::~TCPSubServer() {
}

/* ----------------------------------------------------------------------------------- */
/* TCPSubServer Getters                                                                  */
/* ----------------------------------------------------------------------------------- */
ServerConfig& TCPSubServer::GetConfig() const {
	return _config;
}

// handles a client connection
void TCPSubServer::HandleRequest(int clientSocket) {
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