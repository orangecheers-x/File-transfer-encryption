//
// Created by 高坂穗乃果 on 2022/4/18.
//

#include "DESEncryption.h"
#include <iostream>
std::bitset<48> DESEncryption::keys[16] = {0};


void DESEncryption::Encrypt64(unsigned char *dst, const unsigned char* src, const unsigned char *key, bool isdecryption)
{
    std::bitset<64> bsrc;
    for(int i = 0;i < 64;i++)
    {
        bsrc[i] = ((src[i>>3]>>(i&7))&1);
    }
    std::bitset<64> bkey;
    for(int i = 0;i < 64;i++)
    {
        bkey[i] = ((key[i>>3]>>(i&7))&1);
    }
    std::bitset<64> bdst;
    Encrypt64(bdst, bsrc, bkey, isdecryption);

    //std::cout << "src::" << bsrc << std::endl;
    //std::cout << "dst::" << bdst << std::endl;
    for(int i = 0;i < 8;i++)
    {
        dst[i] = 0;
        for(int j = 0;j < 8;j++)
            dst[i] |= (bdst[(i << 3)|j] << j);
    }
    return ;
}

void DESEncryption::Encrypt64(std::bitset<64> &dst, const std::bitset<64> &src, const std::bitset<64> &key, bool isdecryption) {
    //std::cout << "!" << std::endl;
    generatekeys(key, isdecryption);
    std::bitset<64> t;
    performpermutation(t, src, IP);
    std::bitset<32> a, b;
    std::bitset<32> t32;
    for(int i = 0;i < 32;i++) a[i] = t[i];
    for(int i = 0;i < 32;i++) b[i] = t[i+32];
    for(int i = 0;i < 16;i++)
    {
        F(t32, b, keys[i]);
        t32 ^= a;
        a = b;
        b = t32;
    }
    swap(a, b);
    for(int i = 0;i < 32;i++) t[i] = a[i];
    for(int i = 0;i < 32;i++) t[i+32] = b[i];
    performpermutation(dst, t, IPinv);
    return ;
}

void DESEncryption::DES3_64(std::bitset<64> &dst, const std::bitset<64> &src, const std::bitset<192> &key, bool isdecryption)
{
    std::bitset<64> key3s[3];
    for(int i = 0;i < 192;i++)
        key3s[i/64][i%64] = key[i];
    if(isdecryption) std::reverse(key3s, key3s+3);
    std::bitset<64> t, f;
    f = src;
    for(int i = 0;i < 3;i++)
        Encrypt64(t, f, key3s[i], isdecryption), f = t;
    dst = t;
    return ;
}

void DESEncryption::generatekeys(const std::bitset<64> &key, bool isdecryption) {
    std::bitset<28> z, y;
    performpermutation(z, key, PC1z);
    performpermutation(y, key, PC1y);
    //std::cout << z << "?" << y << std::endl;
    for(int i = 0;i < 16;i++)
    {
        z = ((z << shift[i]) | (z >> (28 - shift[i])));
        y = ((y << shift[i]) | (y >> (28 - shift[i])));
        std::bitset<56> tb;
        for(int j = 0;j < 28;j++) tb[j] = z[j];
        for(int j = 0;j < 28;j++) tb[j+28] = y[j];
        //std::cout << tb << "???" << std::endl;
        //std::cout << i << "??" << std::endl;
        performpermutation(keys[i], tb, PC2);
    }
    if(isdecryption) std::reverse(keys, keys+16);
    return ;
}

template<size_t Ndst, size_t Nsrc>
void DESEncryption::performpermutation(std::bitset<Ndst> &dst, const std::bitset<Nsrc> &src,  const int permutation[]) {
    for(int i = 0;i < Ndst;i++)
    {
        dst[i] = src[permutation[i]-1];
    }
    return ;
}

void DESEncryption::F(std::bitset<32> &dst, std::bitset<32> &x, std::bitset<48>& key) {
    std::bitset<48> e;
    performpermutation(e, x, E);
    e ^= key;
    std::bitset<32> t32;
    for(int i = 0;i < 8;i++)
    {
        int h = (e[i] | (e[i+5]<<1));
        int l = ((e[i+1]) | (e[i+2] << 1) | (e[i+3] << 2) | (e[i+4] << 3));
        int ind = ((h << 4) | l);
        for(int j = 0;j < 3;j++)
            t32[(i << 2) | j] = ((S[i][ind] >> j) & 1);
    }
    performpermutation(dst, t32, P);
    return ;
}
