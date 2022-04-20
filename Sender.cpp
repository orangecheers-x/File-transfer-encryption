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

void Sender::sendbuf(char *buf, int len) const
{
    long sent = 0;
    while((sent += send(ss, buf+sent, (len-sent)*sizeof(char),0)) < len);
}

void Sender::recvbuf(char *buf, int len) const
{
    long sent = 0;
    while((sent += recv(ss, buf+sent, (len-sent)*sizeof(char),0)) < len);
}

void Sender::exchangekeys() {
    gmp_randclass r(gmp_randinit_default);
    r.seed(time(nullptr));
    mpz_class P = DHKeyExchange::getrandomprime(KEY_LENGTH<<3, r);
    mpz_class G = DHKeyExchange::getprimitiveroot(P);

    char bufP[128];
    memset(bufP, 0, sizeof bufP);
    DHKeyExchange::getb(bufP, P);
    sendbuf(bufP, KEY_LENGTH);

    memset(bufP, 0, sizeof bufP);
    DHKeyExchange::getb(bufP, G);
    sendbuf(bufP, KEY_LENGTH);

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
void Sender::sendfile(const char* filename) const {
    std::ifstream infile(filename, std::ios::in|std::ios::binary);
    std::filesystem::path p{filename};
    unsigned long filesize = std::filesystem::file_size(p);
    send(ss, &filesize, sizeof(unsigned long), 0);
    std::cout << "Total File Size: " << filesize << " Bytes" << std::endl;
    char sendbuf[BUF_SIZE];
    while(filesize > 0) {
        infile.read(sendbuf, sizeof(sendbuf));
        long readsize = 0;
        readsize = infile.gcount();
        //long crysize = ((((readsize-1) >> 6) + 1)<<6);
        filesize -= readsize;
        char tbuf[BUF_SIZE];
        char tkey[24];
        DHKeyExchange::getb(tkey, dh.getkey());
        for(int i = 0;i < BUF_SIZE && i < readsize;i += 8)
            DESEncryption::DES3_64((unsigned char*)tbuf+i, (unsigned char*)sendbuf+i, (unsigned char*)tkey, false);

        long sent = 0;
        while((sent += send(ss, tbuf+sent, BUF_SIZE-sent, 0)) < ((((readsize-1) >> 3) + 1) << 3));
        //std::cout << "!" << 1 << std::endl;
        std::cout << filesize << " Bytes waiting for sent..." << std::endl;
    }
    infile.close();

}

Sender::~Sender() {
    std::cout << "Closed!" << std::endl;
    close(ss);
}