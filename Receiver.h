//
// Created by 高坂穗乃果 on 2022/4/19.
//

#ifndef DSJ_RECEIVER_H
#define DSJ_RECEIVER_H

#include "DHKeyExchange.h"

class Receiver {
public:
    constexpr static int BUF_SIZE = 4096;
    constexpr static int KEY_LENGTH_BYTES = 24;
    int ss, conn{};
    Receiver();
    ~Receiver();
    void bindport(int port) const;
    DHKeyExchange dh;

    void recvbuf(char *buf, int len) const;

    void waitforconnect();

    void recvfile(const char *filename) const;

    void exchangekeys();

    void sendbuf(char *buf, int len) const;
};


#endif //DSJ_RECEIVER_H
