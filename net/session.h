#pragma once

#include "socket.h"
#include <string>

class IPv4_session_impl {
public:
	IPv4_session_impl();
	IPv4_session_impl(IPv4_session_impl const& other) =  delete;
	IPv4_session_impl& operator=(IPv4_session_impl const& other) = delete;
	IPv4_session_impl(IPv4_session_impl &&other) = delete;
	IPv4_session_impl& operator=(IPv4_session_impl &&other) = delete;
	~IPv4_session_impl() = default;

	void close();
	void send(IPv4_socket socket, std::string str);
protected:
	void send_impl(IPv4_socket socket, const char* buffer, size_t length);

	IPv4_socket sock;
};

class IPv4_client : public IPv4_session_impl {
public:
	IPv4_client();
	IPv4_client(IPv4_client const& other) = delete;
	IPv4_client& operator=(IPv4_client const& other) = delete;
	IPv4_client(IPv4_client &&other) = delete;
	IPv4_client& operator=(IPv4_client && other) = delete;
	~IPv4_client() = default;

	void connect();
};

class IPv4_server : public IPv4_session_impl {
public:
	IPv4_server();
	IPv4_server(IPv4_server const& other) = delete;
	IPv4_server& operator=(IPv4_server const& other) = delete;
	IPv4_server(IPv4_server &&other) = delete;
	IPv4_server& operator=(IPv4_server &&other) = delete;
	~IPv4_server() = default;

	void start();
	void accept();
private:
	IPv4_socket connected_sock;
};