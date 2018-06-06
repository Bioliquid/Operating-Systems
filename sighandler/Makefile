all: sighandler.cpp
	g++ $^ -o sighandler.o

run: sighandler.o
	./sighandler.o

sighandler.o: sighandler.cpp
	g++ $^ -o sighandler.o

clean:
	rm -f *.o