CC = g++
cppstd = -std=c++11
common = ../Common

all: ServerMain.cpp Server.o
	$(CC) $(cppstd) ServerMain.cpp Server.o -o chatroom_server

Server.o: Server.cpp Server.h $(common)/Common.h
	$(CC) $(cppstd) -c Server.cpp

clean:
	rm -f *.o chatroom_server