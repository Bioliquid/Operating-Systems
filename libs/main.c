#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

char* ready();
char* steady();

int main() {
	printf("%s\n", ready());
	usleep(1500000);
	printf("%s\n", steady());
	usleep(500000);
	void *dynlib;
	char * (*func)();
	dynlib = dlopen("dynamic_rtlib.so", RTLD_LAZY);
	if (!dynlib){
		fprintf(stderr, "Can't open library dynamic_rtlib.so: %s\n", dlerror());
		return 0;
	};
	func = dlsym(dynlib, "go");
	if (!func) {
		fprintf(stderr, "Can't get function: %s\n", dlerror());
		return 0;
	}
	(*func)();
	dlclose(dynlib);
	return 0;
}
