#include "session.h"
#include <iostream>
#include <string>
#include <optional>

int main(int argc, char **argv) {
	IPv4_server server;
	if (argc == 3) {
		server.init(argv[1], argv[2]);
	} else if (argc != 1) {
		std::cout << "Usage:" << std::endl;
		std::cout << "1.make server" << std::endl;
		std::cout << "2.make server args=\"$ip $port\"" << std::endl;
	}
	server.start();
	while (true) {
		server.accept();
		while (true) {
			std::optional<std::string> s =  server.recv();
			if (!s.has_value()) {
				break;
			}
			std::cout << s.value() << std::endl;
			server.send(s.value());
		}
	}
}