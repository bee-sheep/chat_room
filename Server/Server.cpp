#include<iostream>
#include"Server.h"

Server::Server(){
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    listener = 0;
    epfd = 0;
}

void Server::Init(){
    std::cout<<"init server..."<<std::endl;
    listener = socket(PF_INET, SOCK_STREAM, 0);

    if(listener < 0) { perror("listener error"); exit(-1); }

    if(bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        perror("bind error");
        exit(-1);
    }

    int ret = listen(listener, 5);
    if(ret < 0){
        perror("listen error");
        exit(-1);
    }

    std::cout<<"start to listen: "<<SERVER_IP<<std::endl;

    epfd = epoll_create(EPOLL_SIZE);

    if(epfd < 0){
        perror("epfd error");
        exit(-1);
    }

    addfd(epfd, listener, 1);

}

void Server::Close(){
    close(listener);
    close(epfd);
}

int Server::SendBroadcastMessage(int clientfd){
    char recv_buf[BUF_SIZE];
    char send_buf[BUF_SIZE];
    Msg msg;
    bzero(recv_buf, BUF_SIZE);

    std::cout<<"read from client(clientID = "<<clientfd<<")"<<std::endl;
    int len = recv(clientfd, recv_buf, BUF_SIZE, 0);
    memset(&msg, 0, sizeof(msg));
    memcpy(&msg, recv_buf, sizeof(recv_buf));

    msg.fromID = clientfd;
    msg.type = 0;
    int toi = 0;
    int len1 = strlen(msg.content);
    int k = 0;
    for(int i = 1; i < len1; i++){
        k=i;
        if(isdigit(msg.content[i])){
            toi = toi*10+msg.content[i]-'0';
        }else break;
    }
    if(toi){
        msg.type = 1;
        msg.toID = toi;
        memcpy(msg.content, msg.content+k, sizeof(msg.content));
    }

    if(len == 0){
        close(clientfd);

        cilents_list.remove(clientfd);
        std::cout<<"clientID #"<<clientfd<<" closed"
        <<"\n now there are "<<cilents_list.size()
        <<" clients in chatroom"<<std::endl;
    }else{
        bzero(send_buf, BUF_SIZE);

        if(cilents_list.size() == 1){
            memcpy(msg.content, CAUTION, sizeof(CAUTION));
            memcpy(send_buf, &msg, sizeof(msg));
            send(clientfd, send_buf, sizeof(send_buf), 0);
            return len;
        }

        char format_msg[BUF_SIZE];

        if(msg.type == 0){
            
            sprintf(format_msg, SERVER_MESSAGE, clientfd, msg.content);
            memcpy(msg.content, format_msg, sizeof(format_msg));
            for(auto it: cilents_list){
                if(it != clientfd){
                    memcpy(send_buf, &msg, sizeof(msg));
                    if(send (it, send_buf, sizeof(send_buf), 0) < 0)
                        return -1;
                }
            }
        }else if(msg.type == 1){

            bool private_offine = true;
            sprintf(format_msg, SERVER_PRIVATE_MESSAGE, clientfd, msg.content);
            memcpy(msg.content, format_msg, sizeof(format_msg));
            
            for(auto it: cilents_list){
                if(it == msg.toID){
                    private_offine = false;
                    memcpy(send_buf, &msg, sizeof(msg));
                    if(send (it, send_buf, sizeof(send_buf), 0) < 0)
                        return -1;
                }
            }
            if(private_offine){
                sprintf(format_msg, SERVER_PRIVATE_ERROR_MESSAGE, msg.toID);
                memcpy(msg.content, format_msg, BUF_SIZE);
                memcpy(send_buf, &msg, sizeof(msg));

                if(send (msg.fromID, send_buf, sizeof(send_buf), 0) < 0)
                        return -1;
            }
        }
    }return len;
}

void Server::Start(){
    static struct epoll_event events[EPOLL_SIZE];

    Init();

    while(1){
        int epoll_events_counts = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        
        for(int i = 0; i < epoll_events_counts; i++){
            int sockfd = events[i].data.fd;

            if(sockfd == listener){
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof(struct sockaddr_in);

                int clientfd = accept(listener, (struct sockaddr*)&client_address, &client_addrLength);

                std::cout<<"client connnection from: "
                        <<inet_ntoa(client_address.sin_addr)<<":"
                        <<ntohs(client_address.sin_port)<<", clientdf = "
                        <<clientfd<<std::endl;
                
                addfd(epfd, clientfd, true);

                cilents_list.push_back(clientfd);
                std::cout<<"add new clientfd = "<<clientfd<<" to epoll"<<std::endl;
                std::cout<<"now there are "<<cilents_list.size()<<" clients in the chat room"<<std::endl;


                std::cout<<"welcome message"<<std::endl;
                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientfd);
                if(send(clientfd, message, BUF_SIZE, 0) < 0){
                    perror("send error");
                    Close();
                    exit(-1);
                }
            }else{
                if(SendBroadcastMessage(sockfd) < 0){
                    perror("error");
                    Close();
                    exit(-1);
                }
            }
        }
    }
    Close();
}