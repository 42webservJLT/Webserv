#include "TCPServer.hpp"

TCPServer::TCPServer() {}

TCPServer::TCPServer(const TCPServer& other) {
	*this = other;
}

TCPServer& TCPServer::operator=(const TCPServer& other) {
	if (this != &other) {}
}

TCPServer::~TCPServer() {}
