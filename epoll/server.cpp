#include "session.h"
#include <iostream>
#include <string>
#include <optional>

int main(int argc, char **argv) {
	IPv4_server server(4);
	if (argc == 3) {
		server.init(argv[1], argv[2]);
	}
	server.start();
	IPv4_socket sock;
	IPv4_socket cur_sock;
	for (;;) {
		int events = server.wait();
		for (int i = 0; i < events; ++i) {
			cur_sock = IPv4_socket(server.fd_at(i));
			if (static_cast<int>(cur_sock) == static_cast<int>(server)) {
				server.accept();
			} else if (server.ev_at(i) & EPOLLIN) {
				std::optional<std::string> buffer = server.recv(cur_sock);
				if (buffer.has_value()) {
					std::cout << "Received: " << buffer.value() << std::endl;
				}
			}
		}
	}
}