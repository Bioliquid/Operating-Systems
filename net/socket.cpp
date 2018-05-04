#include "socket.h"

IPv4_socket::IPv4_socket() :
	fd(-1)
{}

IPv4_socket::IPv4_socket(int fd_) :
	fd(fd_)
{}

IPv4_socket::IPv4_socket(IPv4_socket const& other) {
	fd = other.fd;
}

IPv4_socket& IPv4_socket::operator=(IPv4_socket const& other) {
	if (this != &other) {
		fd = other.fd;
	}
	return *this;
}

void IPv4_socket::swap(IPv4_socket &other) {
	std::swap(fd, other.fd);
}

IPv4_socket::IPv4_socket(IPv4_socket &&other) {
	this->swap(other);
}
IPv4_socket& IPv4_socket::operator=(IPv4_socket &&other) {
	this->swap(other);
	return *this;
}

IPv4_socket::~IPv4_socket() {
	//this->close();
}

void IPv4_socket::create() {
	fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1) {
		perror("socket() error");
	}
}

void IPv4_socket::set_opt(int optname, int optval) {
	if(::setsockopt(fd, SOL_SOCKET, optname, &optval, sizeof optval) == -1) {
		perror("setsockopt() error");
	}
}

void IPv4_socket::set_opt(int optname, struct timeval tv) {
	if(::setsockopt(fd, SOL_SOCKET, optname, reinterpret_cast<char*>(&tv), sizeof(struct timeval))) {
		perror("setsockopt() error");
	}
}

void IPv4_socket::bind(struct sockaddr_in addr) {
	if(::bind(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(sockaddr)) == -1) {
		perror("bind() error");
	}
}

void IPv4_socket::listen() {
	if(::listen(fd, 1) == -1) {
		perror("listen() error");
	}
}

void IPv4_socket::connect(struct sockaddr_in addr) {
	if(::connect(fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(sockaddr)) == -1) {
        perror("connect() error");
    }
}

IPv4_socket IPv4_socket::accept() {
	int socket_ = ::accept(fd, NULL, NULL);
    if(socket_ == -1) {
        perror("accept() error");
    }
	return IPv4_socket(socket_);
}

void IPv4_socket::close() {
	::close(fd);
}

struct sockaddr_in sock::address(short sin_family, unsigned long addr, unsigned short port) {
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = sin_family;
	serveraddr.sin_addr.s_addr = addr;
	serveraddr.sin_port = htons(port);
	memset(&(serveraddr.sin_zero), '\0', 8);
	return serveraddr;	
}

struct timeval sock::timelimit(int seconds, int miliseconds) {
	struct timeval tv;
	tv.tv_sec = seconds;
	tv.tv_usec = miliseconds;
	return tv;
}

std::string sock::recv(IPv4_socket &socket) {
	int fd = static_cast<int>(socket);

	const size_t buffer_length = 1024;
	char buffer[buffer_length];
	
	//socket.set_opt(SO_RCVTIMEO, sock::timelimit(1, 0));
	int recv_res = ::recv(fd, buffer, buffer_length, 0);	
	if(recv_res == -1) {
		perror("recv() error");
		close(fd);
	} else if (recv_res == 0 && errno != EAGAIN) {
		shutdown(fd, SHUT_RDWR);
        close(fd);
        std::cout << "Connection terminated" << std::endl;
		return "error";
	}
	return std::string(buffer);
}

void sock::send(IPv4_socket &socket, char const* buffer, size_t length) {
	int fd = static_cast<int>(socket);
	if(::send(fd, buffer, length, MSG_NOSIGNAL) == -1) {
		perror("send() error");
	}
}