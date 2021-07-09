#ifndef CHATROOM_COMMON_H
#define CHATROOM_COMMON_H

#include<iostream>
#include<list>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>

//默认服务器ip
#define SERVER_IP "127.0.0.1"
//服务器端口
#define SERVER_PORT 10081

//epoll最大支持的句柄数
#define EPOLL_SIZE 500

//缓冲区大小65535
#define BUF_SIZE 0xFFFF

//新用户登录后的欢迎信息
#define SERVER_WELCOME "Welcome you join to the chat room! Your chat ID is: Client #%d made by cc."

//用户消息前缀
#define SERVER_MESSAGE "Client ID #%d say >> %s"
#define SERVER_PRIVATE_MESSAGE "Client ID #%d say to you privately >> %s"
#define SERVER_PRIVATE_ERROR_MESSAGE "Client ID #%d is not in the chat room yet~"

//退出系统
#define EXIT "EXIT"

//提醒你是唯一在线
#define CAUTION "There is only one in the chat room"
    
//注册新的fd到epollfd中
static void addfd(int epollfd, int fd, bool enable_et){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(enable_et)
        ev.events |= EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)|O_NONBLOCK);
    std::cout<<"fd added to epoll"<<std::endl;
}

struct Msg{
    int type;
    int fromID;
    int toID;
    char content[BUF_SIZE];
};

#endif