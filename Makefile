CC = g++
CFLAGS = -c -Wall

all: s c

s: server.o tcpip.o
	$(CC) -o server server.o tcpip.o 

c: client.o tcpip.o
	$(CC) -o client client.o tcpip.o 

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

tcpip.o: tcpip.cpp
	$(CC) $(CFLAGS) -c tcpip.cpp

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp

clean:
	rm -rf *o client server
