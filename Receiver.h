//
// Created by 高坂穗乃果 on 2022/4/19.
//

#ifndef DSJ_RECEIVER_H
#define DSJ_RECEIVER_H


class Receiver {
public:
    int ss, conn;
    Receiver();
    void bindport(int port);
    void startlisten();
};


#endif //DSJ_RECEIVER_H
