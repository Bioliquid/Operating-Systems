#include <stddef.h>    
#include <stdint.h>    
#include <stdio.h>  
#include <stdlib.h>   
#include <string.h>    
#include <sys/mman.h>

int inc(int a) {
	return a + 1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Unknown command. Function structure: jit [num1] [num2]\n");
        exit(0);
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

	char machine_code[] = { 
        0x55, 
        0x48, 0x89, 0xe5, 
        0x89, 0x7d, 0xfc, 
        0x8b, 0x45, 0xfc, 
        0x83, 0xc0, 0x01, 
        0x5d,
        0xc3
    };

    machine_code[12] = b;

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
    int c = ((int(*)(int))mem)(a);
    printf("%d\n", c);

    if (munmap(mem, sizeof(machine_code)) == -1) {
        perror("munmap");
        return 3;
    }

    return 0;
}