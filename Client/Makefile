CC = g++
CFLAGS = -std=c++11

all: ClientMain.cpp Client.o
	$(CC) $(CFLAGS) ClientMain.cpp Client.o -o chatroom_client


Client.o: Client.cpp Client.h Common/Common.h
	$(CC) $(CFLAGS) -c Client.cpp

clean:
	rm -f *.o chatroom_client