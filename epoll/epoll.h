#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdio>
#include <sys/epoll.h>
#include "socket.h"

class epoll {
public:
	epoll();
	~epoll();
	explicit epoll(int num_events);
	void create(int num_events);
	void ctl(IPv4_socket &sock_, uint32_t events);
	int handle_events();
	int fd_at(int i);
	int ev_at(int i);
private:
	int max_events;
	int epfd;
	struct epoll_event *events;
};