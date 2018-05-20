#include "epoll.h"

epoll::epoll() :
	max_events(0)
{}

epoll::epoll(int num_events) :
	max_events(num_events),
	epfd(epoll_create(num_events))
{
	if (epfd == -1) {
		perror("epoll_create() error");
		exit(EXIT_FAILURE);
	}
}

epoll::~epoll() {
	::close(epfd);
}

void epoll::create(int num_events) {
	if (max_events > 0) {
		return;
	}
	max_events = num_events;
	epfd = epoll_create(num_events);
	if (epfd == -1) {
		perror("epoll_create() error");
		exit(EXIT_FAILURE);
	}
}

void epoll::ctl(IPv4_socket &sock_, uint32_t events) {	
	int sock = static_cast<int>(sock_);
	struct epoll_event socket_event;
    socket_event.data.fd = sock;
    socket_event.events = events;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &socket_event) == -1) {
        perror("epoll_ctl() error");
    }
}

int epoll::handle_events() {
	struct epoll_event evs[max_events];
	int num_events = epoll_wait(epfd, evs, max_events, -1);
    if(num_events == -1) {
        perror("epoll_wait() error");
    }
    events = evs;
	return num_events;
}

int epoll::fd_at(int i) {
	return events[i].data.fd;
}

int epoll::ev_at(int i) {
	return events[i].events;
}