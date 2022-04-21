#include <iostream>
#include <bitset>
#include "DESEncryption.h"
#include "DHKeyExchange.h"
#include "Receiver.h"
#include "Sender.h"

using std::cout;
using std::endl;
using std::cin;


int main(int argc, const char* argv[]) {

    int x;
    cin >> x;
    if(x == 0) {
        cout << "Starting as a Server/Receiver..." << endl;
        Receiver r;
        r.bindport(9999);
        r.waitforconnect();
        r.exchangekeys();
        r.recvfile("rec.pdf");
    }else{
        cout << "Starting as a Client/Sender..." << endl;
        Sender s;
        s.connectserver("82.156.17.131", 9999);
        s.exchangekeys();
        s.sendfile("test.pdf");
    }
    /*mpz_class g = 89434;
    mpz_class p = 3414;
    DHKeyExchange dh(p,g);
    dh.k = 512;
    char c[3];
    dh.getb(c, dh.k);
    cout << (int)c[0] << " " << (int)c[1] << " " << (int)c[2] << endl;
    mpz_class t;
    mpz_import(t.get_mpz_t(), 3, -1, 1, -1, 0, c);
    cout << t.get_str() << endl;*/
    return 0;
}
