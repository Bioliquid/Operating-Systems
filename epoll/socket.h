#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/epoll.h>
#include <time.h>
#include <utility>
#include <optional>
#include <netdb.h>
#include <fcntl.h>

#define PORT 8080

class IPv4_socket {
public:
	IPv4_socket();
	explicit IPv4_socket(int fd);
	IPv4_socket(IPv4_socket const& other) = delete;
	IPv4_socket& operator=(IPv4_socket const& other) = delete;
	IPv4_socket(IPv4_socket &&other);
	IPv4_socket& operator=(IPv4_socket &&other);
	~IPv4_socket();

	operator int() {
		return fd;
	}

	void swap(IPv4_socket &other);
	void reset();

	void create();
	void set_opt(int optname, int optval);
	void set_opt(int optname, struct timeval tv);
	void bind(struct sockaddr_in addr);
	void listen();
	void connect(struct sockaddr_in addr);
	IPv4_socket accept();
	void close();
private:
	int fd;

	fd_set on_read;
	fd_set on_write;
};

namespace sock {
	struct sockaddr_in address(short sin_family, unsigned long addr, unsigned short port);
	struct sockaddr_in address(short sin_family, std::string ip, std::string port);
	struct timeval timelimit(int seconds, int miliseconds);

	std::optional<std::string> recv(IPv4_socket &socket);
	void send(IPv4_socket &socket, char const* buffer, size_t length);
}