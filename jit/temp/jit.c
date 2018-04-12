#include <stddef.h>    
#include <stdint.h>    
#include <stdio.h>     
#include <string.h>    
#include <sys/mman.h>

int sum(int a, int b) {
	return a + b;
}

int main(int argc, char **argv) {
	char machine_code[] = { 0x55, 0x48, 0x89, 0xe5, 0x89, 0x7d, 0xfc, 0x89, 0x75, 0xf8, 0x8b, 0x55, 0xfc, 0x8b, 0x45, 0xf8, 0x01, 0xd0, 0x5d, 0xc3 };

	void *mem = mmap(NULL, sizeof(machine_code), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    memcpy(mem, machine_code, sizeof(machine_code));

    if (mprotect(mem, sizeof(machine_code), PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect");
        return 2;
    }
    int c = ((int(*)(int, int))mem)(2, 3);
    printf("%d", c);

    if (munmap(mem, sizeof(machine_code)) == -1) {
        perror("munmap");
        return 3;
    }

    return 0;
}