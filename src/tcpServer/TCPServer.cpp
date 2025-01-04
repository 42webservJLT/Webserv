#include "TCPServer.hpp"

/* ----------------------------------------------------------------------------------- */
/* TCPServer Constructor & Destructor                                                  */
/* ----------------------------------------------------------------------------------- */
TCPServer::TCPServer() {}

TCPServer::TCPServer(const TCPServer& other) {
	*this = other;
}

TCPServer& TCPServer::operator=(const TCPServer& other) {
	if (this != &other) {}
}

TCPServer::~TCPServer() {}

/* ----------------------------------------------------------------------------------- */
/* TCPServer Start                                                                     */
/* ----------------------------------------------------------------------------------- */
TCPServer::StartServer() {}