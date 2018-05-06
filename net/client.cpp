#include "session.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
	IPv4_client client;
	if (argc == 3) {
		client.init(argv[1], argv[2]);
	}
	client.connect();
	std::string s;
	while (true) {
		std::cin >> s;
		client.send(s);
		s = client.recv();
		std::cout << s << std::endl;
	}
}