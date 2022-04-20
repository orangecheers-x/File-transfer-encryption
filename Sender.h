//
// Created by 高坂穗乃果 on 2022/4/19.
//

#ifndef DSJ_SENDER_H
#define DSJ_SENDER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "DHKeyExchange.h"

class Sender {
public:
    constexpr static int BUF_SIZE = 4096;
    constexpr static int KEY_LENGTH = 24;

    int ss;
    Sender();
    ~Sender();
    void connectserver(const char* ip, int port);
    DHKeyExchange dh;

    void sendbuf(char *buf, int len) const;

    void exchangekeys();

    void sendfile(const char *filename) const;

    void recvbuf(char *buf, int len) const;
};


#endif //DSJ_SENDER_H
