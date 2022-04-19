//
// Created by 高坂穗乃果 on 2022/4/18.
//

#include <iostream>
#include "DHKeyExchange.h"






void DHKeyExchange::generateprivatekey(int len, gmp_randclass &r) {
    c = r.get_z_bits(len);
    return ;
}

mpz_class DHKeyExchange::getpublickey() {
    mpz_class ans;
    mpz_powm(ans.get_mpz_t(), G.get_mpz_t(), c.get_mpz_t(), P.get_mpz_t());
    return ans;
}

void DHKeyExchange::received(const mpz_class& key2) {
    mpz_powm(k.get_mpz_t(), key2.get_mpz_t(), c.get_mpz_t(), P.get_mpz_t());
    return;
}

mpz_class DHKeyExchange::getkey() {
    return k;
}

mpz_class DHKeyExchange::getrandomprime(int len, gmp_randclass &r) {
    mpz_class t = r.get_z_bits(len);
    mpz_class ans;
    mpz_nextprime(ans.get_mpz_t(), t.get_mpz_t());
    return ans;
}

mpz_class DHKeyExchange::getprimitiveroot(const mpz_class &x) {
    mpz_class k = (x-1)/2;
    mpz_class t;
    for(mpz_class i = 2;i <= x-1;i++)
    {
        mpz_powm(t.get_mpz_t(), i.get_mpz_t(), k.get_mpz_t(), x.get_mpz_t());
        if(t != 1) return i;
    }
    return 0_mpz;
}

