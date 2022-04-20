//
// Created by 高坂穗乃果 on 2022/4/19.
//

#ifndef DSJ_SENDER_H
#define DSJ_SENDER_H

#include <sys/socket.h>
#include <netinet/in.h>

class Sender {
public:
    int ss;
    Sender();
    void connectserver(const char* ip, int port);
    void sendfile();
};


#endif //DSJ_SENDER_H
