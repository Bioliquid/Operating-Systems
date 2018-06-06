#include <iostream>
#include <vector>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/uio.h>
#include <stdio.h>
#include <ucontext.h>
#include <sys/types.h>
#include <bits/sigcontext.h>
#include <features.h>
#include <stdbool.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <errno.h>

sigjmp_buf sig_buf;

char hex[] = "0123456789abcdef";

enum { R8, R9, R10, R11, R12, R13, R14, R15, RDI, RSI, RBP,RBX, RDX, RAX, RCX, RSP };
char const *regs[16] = { "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "RDI", "RSI", "RBP", "RBX", "RDX", "RAX", "RCX", "RSP" };

void get_hex(unsigned long long num, char *hex_value) {
	hex_value[0] = '0'; hex_value[1] = 'x'; 
	for (size_t i = 0; i < 16; ++i) {
		hex_value[i + 2] = hex[num % 16];
		num /= 16;
	}
	hex_value[18] = '\0';
}

inline void new_line() {
	write(1, "\n", 1);
}

void print(char const *buffer) {
	size_t len = strlen(buffer);

	while (len > 0) {
		ssize_t res = write(1, buffer, len);
		if (res == -1) {
			char *err = strerror(errno);
			write(2, err, strlen(err));
			exit(EXIT_FAILURE);
		}
		buffer += res;
		len -= res;
	}
}

void print(char c) {
	char buf[] = { c, '\0' };
	write(1, buf, 2);
}

inline void new_section() {
	print("----------------------------");
	new_line();
}

void base_handler(int signum) {
	siglongjmp(sig_buf, 1);
}

void handler(int signum, siginfo_t *siginfo, void *context) {
	new_section();

	char const welcome_msg[] = "Caught a segmentation fault\n";
	print(welcome_msg);

	new_section();

	print("Address:");
	new_line();

	char address[19];
	unsigned long long addr = reinterpret_cast<unsigned long long>(siginfo->si_addr);
	get_hex(addr, address);
	print("\tAddress = ");
	print(address);
	new_line();

	new_section();

	print("Registers:\n");

	mcontext_t mc = reinterpret_cast<ucontext_t*>(context)->uc_mcontext;
	mcontext_t *mcontext = &mc;
	for (size_t reg = R8; reg <= RSP; ++reg) {
		print("\t");
		print(regs[reg]);
		print(" = ");
		get_hex(static_cast<unsigned long long>(mcontext->gregs[reg]), address);
		print(address);
		new_line();
	}

	new_section();

	print("Memory dump:\n");

	struct sigaction action;
	memset(&action, 0, sizeof(action));

	action.sa_handler = base_handler;
	action.sa_flags = SA_NODEFER;
	
	if(sigaction(SIGSEGV, &action, NULL) == -1) {
		print(strerror(errno));
		exit(EXIT_FAILURE);
	} 

	int const range = 64;

	for (int i = -range; i < range; i += 16) {
		new_line();
		for (int j = i; j < i + 16; ++j, ++addr) {
			if (j == 0) {
				print("[");
			}

			if (sigsetjmp(sig_buf, 0)) {
				print("**");
			} else {
				unsigned char c = *reinterpret_cast<unsigned char*>(addr);
				print(hex[c / 16]);
				print(hex[c % 16]);
			}

			if (j == 0) {
				print("]");
			}

			print(" ");
		}
	}

	new_line();

	exit(EXIT_FAILURE);
}

int main(int argc, char * argv[]) {
	struct sigaction action;
	memset(&action, 0, sizeof(action));

	action.sa_sigaction = handler;
	action.sa_flags = SA_SIGINFO | SA_NODEFER;
	
	if(sigaction(SIGSEGV, &action, NULL) == -1) {
		print(strerror(errno));
		exit(EXIT_FAILURE);
	} 
	char *str = "foobar";
	str[0] = 'b';
	return 0;
}