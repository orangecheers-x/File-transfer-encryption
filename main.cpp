#include <iostream>
#include <bitset>
#include "DESEncryption.h"
#include "DHKeyExchange.h"
#include "Receiver.h"
#include "Sender.h"

using std::cout;
using std::endl;
using std::cin;


int main() {
    int x;
    cin >> x;
    if(x == 0) {
        cout << "Starting as a Server/Receiver..." << endl;
        Receiver r;
        r.bindport(9999);
        r.startlisten();
    }else{
        cout << "Starting as a Client/Sender..." << endl;
        Sender s;
        s.connectserver("127.0.0.1", 9999);
        s.sendfile();
    }
    return 0;
}
