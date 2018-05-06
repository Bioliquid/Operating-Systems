#include "session.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
	IPv4_server server;
	if (argc == 3) {
		server.init(argv[1], argv[2]);
	}
	server.start();
	server.accept();
	while (true) {
		std::string s =  server.recv();
		std::cout << s << std::endl;
		server.send(s);
	}

}