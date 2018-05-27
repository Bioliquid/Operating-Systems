CXXFLAGS+=-Wall -std=c++17

all: socket.o epoll.o session.o client.o server.o
	g++ socket.o epoll.o session.o client.o -o sclient.o
	g++ socket.o epoll.o session.o server.o -o sserver.o

client: sclient.o
	./sclient.o $(args)

server: sserver.o
	./sserver.o $(args)

server.o: server.cpp
	g++ -c $^ $(CXXFLAGS) -o server.o

client.o: client.cpp
	g++ -c $^ $(CXXFLAGS) -o client.o
	
session.o: session.cpp
	g++ -c $^ $(CXXFLAGS) -o session.o

epoll.o: epoll.cpp
	g++ -c $^ $(CXXFLAGS) -o epoll.o
	
socket.o: socket.cpp
	g++ -c $^ $(CXXFLAGS) -o socket.o

clean:
	rm -f *.o