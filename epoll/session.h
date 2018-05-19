#pragma once

#include "socket.h"
#include "epoll.h"
#include <string>

class IPv4_session_impl {
public:
	IPv4_session_impl();
	IPv4_session_impl(char *ip, char *prt);
	IPv4_session_impl(int num_events);
	IPv4_session_impl(char *ip, char *prt, int num_events);
	IPv4_session_impl(IPv4_session_impl const& other) =  delete;
	IPv4_session_impl& operator=(IPv4_session_impl const& other) = delete;
	IPv4_session_impl(IPv4_session_impl &&other) = delete;
	IPv4_session_impl& operator=(IPv4_session_impl &&other) = delete;
	~IPv4_session_impl() = default;

	void init(char *ip, char *prt);
	void close();
	std::optional<std::string> recv(IPv4_socket &socket);
	void send(IPv4_socket &socket, std::string str);

	int wait();
	int fd_at(int i);
	int ev_at(int i);

	operator int() {
		return static_cast<int>(sock);
	}
protected:
	void send_impl(IPv4_socket &socket, const char* buffer, size_t length);

	IPv4_socket sock;
	epoll epfd;
	std::string ip_address;
	std::string port;
	int const max_events;
};

class IPv4_client : virtual public IPv4_session_impl {
public:
	IPv4_client();
	IPv4_client(char *ip, char *prt);
	IPv4_client(int num_events);
	IPv4_client(char *ip, char *prt, int num_events);
	IPv4_client(IPv4_client const& other) = delete;
	IPv4_client& operator=(IPv4_client const& other) = delete;
	IPv4_client(IPv4_client &&other) = delete;
	IPv4_client& operator=(IPv4_client && other) = delete;
	~IPv4_client() = default;

	void connect();
	void send(std::string str);
	std::optional<std::string> recv();
};

class IPv4_server : virtual public IPv4_session_impl {
public:
	IPv4_server();
	IPv4_server(char *ip, char *prt);
	IPv4_server(int num_events);
	IPv4_server(char *ip, char *prt, int num_events);
	IPv4_server(IPv4_server const& other) = delete;
	IPv4_server& operator=(IPv4_server const& other) = delete;
	IPv4_server(IPv4_server &&other) = delete;
	IPv4_server& operator=(IPv4_server &&other) = delete;
	~IPv4_server() = default;

	void start();
	void accept();
	void send(IPv4_socket &sock, std::string str);
	std::optional<std::string> recv(IPv4_socket &sock);
};