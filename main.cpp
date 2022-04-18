#include <iostream>
#include <bitset>
#include "DESEncryption.h"

int main() {
    std::bitset<64> dst, src, key;
    //std::string s = "OHAKONOH";
    std::string s = "HONOKAHO";
    for(int i = 0;i < 8;i++)
    {
        for(int j = 7;j >= 0;j--)
        {
            key[(i<<3)|(7-j)] = ((s[i]>>j)&1);
        }
    }
    std::cout << "key:" << key << std::endl;
    src = 999999999ll;
    std::cout << "src:" << src << std::endl;
    DESEncryption::Encrypt64(dst, src, key, false);
    for(int i = 0;i < 16;i++)
    {
        std::cout << "key" << i << ":" << DESEncryption::keys[i] << std::endl;
    }
    std::cout << "=====" << std::endl;
    std::cout << dst << std::endl;
    src = dst;
    DESEncryption::Encrypt64(dst, src, key, true);
    std::cout << "=====" << std::endl;
    std::cout << "dst:" << dst << std::endl;

    /*std::bitset<8> b = 35;
    std::cout << b << std::endl;
    std::cout << ((b << 5) | (b >> 3)) << std::endl;*/
    return 0;
}
