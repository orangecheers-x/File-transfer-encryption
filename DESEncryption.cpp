//
// Created by 高坂穗乃果 on 2022/4/18.
//

#include "DESEncryption.h"

constexpr void DESEncryption::Encrypt64(std::bitset<64> &dst, const std::bitset<64> &src, const std::bitset<64> &key) {
    generatekeys(key);
}

void DESEncryption::generatekeys(const std::bitset<64> &key) {
    std::bitset<28> z, y;
    performpermutation(z, key, PC1z);
    performpermutation(y, key, PC1y);
    for(int i = 0;i < 16;i++)
    {
        z = ((z << shift[i]) | (z >> (28 - i)));
        y = ((y << shift[i]) | (y >> (28 - i)));
        std::bitset<56> tb;
        for(int j = 0;i < 28;i++) tb[j] = z[j];
        for(int j = 0;i < 28;i++) tb[j+28] = z[j];
        performpermutation(keys[i], tb, PC2);
    }
    return ;
}

template<size_t Ndst, size_t Nsrc>
constexpr void DESEncryption::performpermutation(std::bitset<Ndst> &dst, const std::bitset<Nsrc> &src, const int *permutation) {
    for(int i = 0;i < Ndst;i++)
    {
        dst[i] = src[permutation[i]-1];
    }
    return ;
}
