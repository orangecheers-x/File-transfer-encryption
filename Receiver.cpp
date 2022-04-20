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
    sockaddr_in clientaddr{};
    socklen_t length = sizeof(clientaddr);
    ss = socket(AF_INET, SOCK_STREAM, 0);

}

void Receiver::bindport(int port) const
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

void Receiver::recvbuf(char *buf, int len) const
{
    long sent = 0;
    while((sent += recv(conn, buf+sent, (len-sent)*sizeof(char),0)) < len);
}

void Receiver::sendbuf(char *buf, int len) const
{
    long sent = 0;
    while((sent += send(conn, buf+sent, (len-sent)*sizeof(char),0)) < len);
}


void Receiver::recvfile(const char* filename) const {
    char buffer[BUF_SIZE];
    char sendbuf[BUF_SIZE];
    strcpy(sendbuf, "ww");
    std::ofstream outfile(filename, std::ios::out|std::ios::binary);

    long filesize;
    recv(conn, &filesize, sizeof(unsigned long), 0);
    std::cout << "Total File Size: " << filesize << " Bytes" << std::endl;
    while(filesize > 0)
    {
        memset(buffer, 0, sizeof buffer);
        long recved = 0;
        while((recved += recv(conn, buffer+recved, BUF_SIZE-recved, 0)) < std::min((((filesize-1) >> 3) + 1) << 3, (long)BUF_SIZE));

        char tbuf[BUF_SIZE];
        char tkey[24];
        DHKeyExchange::getb(tkey, dh.getkey());
        for(int i = 0;i < recved;i += 8)
        {
            DESEncryption::DES3_64((unsigned char*)tbuf+i, (unsigned char*)buffer+i, (unsigned char*)tkey, true);
        }
        outfile.write(tbuf, std::min(filesize, (long)BUF_SIZE) * sizeof(char));
        filesize -= recved;
        std::cout << std::max(0l, filesize) << " Bytes remaining..." << std::endl;
    }
    outfile.close();
}

void Receiver::exchangekeys() {
    gmp_randclass r(gmp_randinit_default);
    r.seed(time(nullptr));
    mpz_class P, G;
    char bufP[128];
    memset(bufP, 0, sizeof bufP);
    recvbuf(bufP, KEY_LENGTH_BYTES);
    P = DHKeyExchange::getmpz(bufP, KEY_LENGTH_BYTES);
    recvbuf(bufP, KEY_LENGTH_BYTES);
    G = DHKeyExchange::getmpz(bufP, KEY_LENGTH_BYTES);

    std::cout << "P: " << P.get_str(16) << std::endl;
    std::cout << "G: " << G.get_str(16) << std::endl;

    dh.init(P, G);
    dh.generateprivatekey(128, r);

    memset(bufP, 0, 128 * sizeof(char));
    DHKeyExchange::getb(bufP, dh.getpublickey());
    std::cout << "public key:" << DHKeyExchange::getmpz(bufP, 128).get_str(16) << std::endl;
    sendbuf(bufP, 128);

    memset(bufP, 0, 128 * sizeof(char));
    recvbuf(bufP, 128);
    dh.received(DHKeyExchange::getmpz(bufP, 128));

    std::cout << "exchanged key:" << dh.getkey().get_str(16) << std::endl;
}

void Receiver::waitforconnect() {
    if(listen(ss, 20) == -1)
    {
        std::cerr << "Listen Error!" << std::endl;
        exit(1);
    }
    sockaddr_in client_addr{};
    socklen_t slen = sizeof(client_addr);
    conn = accept(ss, (sockaddr*) &client_addr, &slen);
    if(conn < 0)
    {
        std::cerr << "Connect Error!" << std::endl;
        exit(1);
    }
}

Receiver::~Receiver() {
    std::cout << "Closed!" << std::endl;
    close(conn);
    close(ss);
}
