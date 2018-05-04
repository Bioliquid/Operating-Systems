CXXFLAGS+=-Wall -std=c++11

all: socket.o session.o client.o server.o
g++ socket.o session.o client.o -o sclient.o
g++ socket.o session.o server.o -o sserver.o

client: sclient.o
./sclient.o

server: sserver.o
./sserver.o

server.o: server.cpp
g++ -c $^ $(CXXFLAGS) -o server.o

client.o: client.cpp
g++ -c $^ $(CXXFLAGS) -o client.o

session.o: session.cpp
g++ -c $^ $(CXXFLAGS) -o session.o

socket.o: socket.cpp
g++ -c $^ $(CXXFLAGS) -o socket.o

clean:
rm -f *.o