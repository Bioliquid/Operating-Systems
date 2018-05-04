#include "session.h"
#include <iostream>
#include <string>

int main() {
	IPv4_server server;
	server.start();
	while (true) {
		server.accept();
		std::string s =  server.recv();
		std::cout << s << std::endl;
		server.send(s);
	}
	server.close();
}