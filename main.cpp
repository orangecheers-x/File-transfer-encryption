#include <iostream>
#include <bitset>
#include "DESEncryption.h"
#include "DHKeyExchange.h"

using std::cout;
using std::endl;


int main() {
    std::bitset<64> b(99);
    std::bitset<192> k(9999999999);
    std::bitset<64> ans, ans2;
    cout << b << endl;
    DESEncryption::DES3_64(ans, b, k, false);
    cout << ans << endl;
    DESEncryption::DES3_64(ans2, ans, k, true);
    cout << ans2 << endl;

    return 0;
}
