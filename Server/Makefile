CC = g++
CFLAGS = -std=c++11

all: ServerMain.cpp Server.o
	$(CC) $(CFLAGS) ServerMain.cpp Server.o -o chatroom_server

Server.o: Server.cpp Server.h Common/Common.h
	$(CC) $(CFLAGS) -c Server.cpp

clean:
	rm -f *.o chatroom_server