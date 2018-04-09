#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/types.h>

bool is_hex(char c) {
	if (isdigit(c) || (c >= 'a' && c <= 'f')) {
		return true;
	}
	return false;
}

int main() {
	FILE *f = fopen("disassemble.ds", "r");
	FILE *nf = fopen("machine.mc", "w");
	char line[256];
	fprintf(nf, "{ ");
	while (fscanf(f, "%s", line) == 1) {
		if (strlen(line) == 2) {
			if(is_hex(line[0]) && is_hex(line[1])) { 
				fprintf(nf, "0x%s, ", line);
			}
		}
	}
	fprintf(nf, "};");
}