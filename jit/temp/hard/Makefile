all: jit.o

run: jit.o
	./jit.o

jit.o: jit.c
	gcc jit.c -o jit.o

clean:
	rm -f *.o