all: main.o static_lib.a dynamic_lib.so dynamic_rtlib.so
	gcc -o main main.o -L. static_lib.a -ldl dynamic_lib.so -Wl,-rpath,.

main.o: main.c
	gcc -c main.c

static_lib.a: static_lib.o
	ar cr static_lib.a static_lib.o

static_lib.o: static_lib.c
	gcc -c static_lib.c

dynamic_lib.so: dynamic_lib.o
	gcc -shared -o dynamic_lib.so dynamic_lib.o

dynamic_lib.o: dynamic_lib.c
	gcc -c -fPIC dynamic_lib.c

dynamic_rtlib.so: dynamic_rtlib.o
	gcc -shared -o dynamic_rtlib.so dynamic_rtlib.o

dynamic_rtlib.o: dynamic_rtlib.c
	gcc -c -fPIC dynamic_rtlib.c

clean:
	-rm -f *.o *.so *.a