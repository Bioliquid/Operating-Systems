#include "session.h"
#include <iostream>
#include <string>

int main() {
	IPv4_client client;
	client.connect();
	std::string s;
	while (true) {
		std::cin >> s;
		client.send(s);
		s = client.recv();
		std::cout << s << std::endl;
	}
	client.close();
}