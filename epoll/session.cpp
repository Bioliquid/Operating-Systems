#include "session.h"

IPv4_session_impl::IPv4_session_impl() :
	sock(),
	ip_address(""),
	port(""),
	max_events(1)
{}

IPv4_session_impl::IPv4_session_impl(char *ip, char *prt) :
	sock(),
	ip_address(ip),
	port(prt),
	max_events(1)
{}

IPv4_session_impl::IPv4_session_impl(int num_events) :
	sock(),
	ip_address(""),
	port(""),
	max_events(num_events)
{}

IPv4_session_impl::IPv4_session_impl(char *ip, char *prt, int num_events) :
	sock(),
	ip_address(ip),
	port(prt),
	max_events(num_events)
{}

void IPv4_session_impl::init(char *ip, char *prt) {
	ip_address = std::string(ip);
	port = std::string(prt);
}

void IPv4_session_impl::close() {
	sock.close();
}

void IPv4_session_impl::send_impl(IPv4_socket &socket_, const char* buffer, size_t length) {
	sock::send(socket_, buffer, length + 1);
}

void IPv4_session_impl::send(IPv4_socket &socket, std::string str) {
	this->send_impl(socket, str.c_str(), str.size());
}

std::optional<std::string> IPv4_session_impl::recv(IPv4_socket &socket) {
	return sock::recv(socket);
}

IPv4_client::IPv4_client() : 
	IPv4_session_impl()
{}

IPv4_client::IPv4_client(char *ip, char *prt) :
	IPv4_session_impl(ip, prt)	
{}

IPv4_client::IPv4_client(int num_events) : 
	IPv4_session_impl(num_events)
{}

IPv4_client::IPv4_client(char *ip, char *prt, int num_events) :
	IPv4_session_impl(ip, prt, num_events)	
{}

IPv4_server::IPv4_server() : 
	IPv4_session_impl()
{}

IPv4_server::IPv4_server(char *ip, char *prt) :
	IPv4_session_impl(ip, prt)	
{}

IPv4_server::IPv4_server(int num_events) : 
	IPv4_session_impl(num_events)
{}

IPv4_server::IPv4_server(char *ip, char *prt, int num_events) :
	IPv4_session_impl(ip, prt, num_events)	
{}

void IPv4_server::start() {
	sock.create();
	sock.set_opt(SO_REUSEADDR, 1);
	if (ip_address == "") {
		sock.bind(sock::address(AF_INET, INADDR_LOOPBACK, PORT));
	} else {
		sock.bind(sock::address(AF_INET, ip_address, port));
	}
	sock.listen();
	sock.set_opt(SO_RCVTIMEO, sock::timelimit(3, 0));
	epfd.create(max_events);
	epfd.ctl(sock, EPOLLIN);
	std::cout << "Server is working" << std::endl;
}

void IPv4_server::accept() {
	IPv4_socket connected_sock = sock.accept();
	epfd.ctl(connected_sock, EPOLLIN);
	connected_sock.reset();
	std::cout << "Connection accepted" << std::endl;
}

void IPv4_server::send(IPv4_socket &connected_sock, std::string str) {
	if (connected_sock == -1) {
		std::cerr << "no connected socket" << std::endl;
		return;
	}
	IPv4_session_impl::send(connected_sock, str);
}

std::optional<std::string> IPv4_server::recv(IPv4_socket &connected_sock) {
	if (connected_sock == -1) {
		std::cerr << "no connected socket" << std::endl;
		return {};
	}
	std::optional<std::string> recv = IPv4_session_impl::recv(connected_sock);
	if (!recv.has_value()) {
		connected_sock.reset();
		return {};
	}
	return recv;
}

void IPv4_client::connect() {
	sock.create();

	epfd.create(max_events);

	int connection;

    if (ip_address == "") {
		connection = sock.connect(sock::address(AF_INET, INADDR_LOOPBACK, PORT));
	} else {
		connection = sock.connect(sock::address(AF_INET, ip_address, port));
	}
	if (connection) {
		epfd.ctl(sock, EPOLLIN | EPOLLOUT);
	} else {
		epfd.ctl(sock, EPOLLIN);
	}
} 

void IPv4_client::send(std::string str) {
	IPv4_session_impl::send(sock, str);
}

std::optional<std::string> IPv4_client::recv() {
	return IPv4_session_impl::recv(sock);
}

int IPv4_session_impl::wait() {
	return epfd.handle_events();	
}

int IPv4_session_impl::fd_at(int i) {
	return epfd.fd_at(i);
}

int IPv4_session_impl::ev_at(int i) {
	return epfd.ev_at(i);
}
