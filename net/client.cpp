#include "session.h"
#include <iostream>
#include <string>
#include <optional>

int main(int argc, char **argv) {
	IPv4_client client;
	if (argc == 3) {
		client.init(argv[1], argv[2]);
	} else if (argc != 1) {
		std::cout << "Usage:" << std::endl;
		std::cout << "1.make client" << std::endl;
		std::cout << "2.make client args=\"$ip $port\"" << std::endl;
	}
	client.connect();
	std::string input;
	std::optional<std::string> output;
	while (true) {
		std::cin >> input;
		client.send(input);
		output = client.recv();
		if (!output.has_value()) {
			break;
		}
		std::cout << output.value() << std::endl;
	}
}