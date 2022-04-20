//
// Created by 高坂穗乃果 on 2022/4/19.
//

#include "Receiver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "DESEncryption.h"
#include <algorithm>

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
    char buffer[BUF_SIZE];
    char sendbuf[BUF_SIZE];
    strcpy(sendbuf, "ww");
    std::ofstream outfile("rec.pdf", std::ios::out|std::ios::binary);

    long filesize;
    recv(conn, &filesize, sizeof(unsigned long), 0);
    std::cout << filesize << "!" << std::endl;
    while(filesize > 0)
    {
        memset(buffer, 0, sizeof buffer);
        long recved = 0;
        while((recved += recv(conn, buffer+recved, BUF_SIZE-recved, 0)) < std::min((((filesize-1) >> 3) + 1) << 3, (long)BUF_SIZE));

        char tbuf[BUF_SIZE];
        unsigned char tkey[8];
        *(long long*) tkey = 0x0123456789ABCDEFll;
        for(int i = 0;i < recved;i += 8)
        {
            DESEncryption::Encrypt64((unsigned char*)tbuf+i, (unsigned char*)buffer+i, tkey, true);
        }
        outfile.write(tbuf, std::min(filesize, (long)BUF_SIZE) * sizeof(char));
        filesize -= recved;
    }
    close(conn);
    close(ss);
    outfile.close();
}
