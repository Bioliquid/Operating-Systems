CXXFLAGS+=-fsanitize=address -Wall
CXX=gcc
CPP=g++

all: jit.o simple.o parser.o

run: jit.o
	./jit.o

mcode: parser.o
	./parser.o

jit.o: jit.c simple.cpp
	$(CPP) $^ $(CXXFLAGS) -o jit.o
	gdb -batch -ex 'file jit.o' -ex 'disassemble /r go_crazy' > "disassemble.ds"

parser.o: parser.c
	$(CXX) $^ $(CXXFLAGS) -o parser.o

clean:
	rm -f *.o