#include "session.h"
#include <iostream>
#include <string>
#include <optional>

int main(int argc, char **argv) {
	IPv4_client client(4);
	if (argc == 3) {
		client.init(argv[1], argv[2]);
	}
	client.connect();
	for (;;) {
		int events = client.wait();
		for (int i = 0; i < events; ++i) { 
			if (client.ev_at(i) & EPOLLOUT) {
				std::string s;
				std::cin >> s;
				client.send(s);
			}
		}
	}
}