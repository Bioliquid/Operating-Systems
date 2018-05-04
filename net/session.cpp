#include "session.h"

IPv4_session_impl::IPv4_session_impl() :
	sock()
{}

void IPv4_session_impl::close() {
	sock.close();
}

void IPv4_session_impl::send_impl(IPv4_socket socket_, const char* buffer, size_t length) {
	sock::send(socket_, buffer, length + 1);
}

void IPv4_session_impl::send(IPv4_socket socket, std::string str) {
	this->send_impl(socket, str.c_str(), str.size() + 1);
}

IPv4_client::IPv4_client() : 
	IPv4_session_impl()
{}

IPv4_server::IPv4_server() : 
	IPv4_session_impl()
{}

void IPv4_server::start() {
	sock.create();
	sock.set_opt(SO_REUSEADDR, 1);
	sock.bind(sock::address(AF_INET, INADDR_ANY, PORT));
	sock.listen();
	std::cout << "Server is working" << std::endl;
}

void IPv4_server::accept() {
	connected_sock = sock.accept();
	std::cout << "Connection accepted" << std::endl;
}

void IPv4_client::connect() {
	sock.create();
    sock.connect(sock::address(AF_INET, INADDR_ANY, PORT));
} 