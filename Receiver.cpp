//
// Created by 高坂穗乃果 on 2022/4/19.
//

#include "Receiver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

Receiver::Receiver() {
    ss = socket(AF_INET, SOCK_STREAM, 0);

    if(listen(ss, 20) == -1)
    {
        std::cerr << "Listen Error!" << std::endl;
        exit(1);
    }
    sockaddr_in clientaddr;
    socklen_t length = sizeof(clientaddr);
    ss = socket(AF_INET, SOCK_STREAM, 0);

}

void Receiver::bindport(int port)
{
    sockaddr_in server_sockaddr{};
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(9999);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(ss, (sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) == -1)
    {
        std::cerr << "Bind Error!" << std::endl;
        exit(1);
    }
    return ;
}

void Receiver::startlisten() {
    if(listen(ss, 20) == -1)
    {
        std::cerr << "Listen Error!" << std::endl;
        exit(1);
    }
    sockaddr_in client_addr;
    socklen_t slen = sizeof(client_addr);
    conn = accept(ss, (sockaddr*) &client_addr, &slen);
    if(conn < 0)
    {
        std::cerr << "Connect Error!" << std::endl;
        exit(1);
    }
    char buffer[1024];
    char sendbuf[1024];
    strcpy(sendbuf, "ww");
    std::ofstream outfile("rec.pdf", std::ios::out|std::ios::binary);
    unsigned long sum = 0;
    unsigned long filesize;
    recv(conn, &filesize, sizeof(unsigned long), 0);
    std::cout << filesize << "!" << std::endl;
    while(sum < filesize)
    {
        memset(buffer, 0, sizeof buffer);
        ssize_t len = recv(conn, buffer, sizeof buffer, 0);
        if(len == 0) break;
        sum += len;
        std::cout << len << " " << sum << std::endl;
        outfile.write(buffer, len * sizeof(char));
        //send(conn, sendbuf, len , 0);
    }
    close(conn);
    close(ss);
    outfile.close();
}
