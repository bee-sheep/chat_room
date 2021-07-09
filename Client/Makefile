CC = g++
cppstd = -std=c++11
common = ../Common

all: ClientMain.cpp Client.o
	$(CC) $(cppstd) ClientMain.cpp Client.o -o chatroom_client


Client.o: Client.cpp Client.h $(common)/Common.h
	$(CC) $(cppstd) -c Client.cpp

clean:
	rm -f *.o chatroom_client