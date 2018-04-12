CXXFLAGS+=-fsanitize=address -Wall
CXX=gcc

all: jit.o

run: jit.o
	./jit.o

jit.o: jit.c
	$(CXX) $^ $(CXXFLAGS) -o jit.o

clean:
	rm -f *.o