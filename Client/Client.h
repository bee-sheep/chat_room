#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H

#include<string>
#include "../Common/Common.h"

class Client
{
private:
    /* data */
    int sock, pid, epfd, pipe_fd[2];
    bool isClientwork;

    Msg msg;
    char recv_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];

    struct sockaddr_in serverAddr;

public:
    Client(/* args */);

    void Connect();
    void Close();
    void Start();
};


#endif