//
// Created by 高坂穗乃果 on 2022/4/19.
//

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include "Sender.h"
#include "DESEncryption.h"
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
    char sendbuf[BUF_SIZE];
    while(filesize > 0) {
        std::cout << filesize << std::endl;
        infile.read(sendbuf, sizeof(sendbuf));
        long readsize = 0;
        readsize = infile.gcount();
        //long crysize = ((((readsize-1) >> 6) + 1)<<6);
        filesize -= readsize;
        char tbuf[BUF_SIZE];
        unsigned char tkey[8];
        *(long long*) tkey = 0x0123456789ABCDEFll;
        for(int i = 0;i < BUF_SIZE && i < readsize;i += 8)
            DESEncryption::Encrypt64((unsigned char*)tbuf+i, (unsigned char*)sendbuf+i, tkey, false);

        long sent = 0;
        while((sent += send(ss, tbuf+sent, BUF_SIZE-sent, 0)) < ((((readsize-1) >> 3) + 1) << 3));
        //std::cout << "!" << 1 << std::endl;
    }
    close(ss);
    infile.close();
}
