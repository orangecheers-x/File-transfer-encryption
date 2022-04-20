//
// Created by 高坂穗乃果 on 2022/4/19.
//

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "Sender.h"
#include <filesystem>
#include <algorithm>

Sender::Sender() {
    ss = socket(AF_INET, SOCK_STREAM, 0);
}

void Sender::connectserver(const char* ip, int port) {
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);
    if(connect(ss, (sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        std::cerr << "Connect Error" << std::endl;
}


void Sender::sendfile() {
    std::ifstream infile("test.pdf", std::ios::in|std::ios::binary);
    std::filesystem::path p{"test.pdf"};
    unsigned long filesize = std::filesystem::file_size(p);
    send(ss, &filesize, sizeof(unsigned long), 0);
    std::cout << filesize << "!" << std::endl;
    char sendbuf[1024];
    char recvbuf[1024];
    while(filesize > 0) {
        std::cout << filesize << std::endl;
        infile.read(sendbuf, sizeof(sendbuf));
        long readsize = 0;
        readsize = infile.gcount();
        filesize -= readsize;
        long sent = 0;
        while((sent += send(ss, sendbuf+sent, readsize-sent, 0)) < readsize);
        std::cout << "!" << 1 << std::endl;
    }
    close(ss);
    infile.close();
}
