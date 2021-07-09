#ifndef CHATROOM_SERVER_H
#define CHATROOM_SERVER_H

#include "Common/Common.h"

class Server
{
private:
    /* data */
    int SendBroadcastMessage(int clientid);
    struct sockaddr_in serverAddr;
    int listener;
    int epfd;
    std::list<int> cilents_list;

public:
    Server(/* args */);
    
    void Init();
    void Start();
    void Close();

};

#endif