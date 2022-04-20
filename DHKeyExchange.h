//
// Created by 高坂穗乃果 on 2022/4/18.
//

#ifndef DSJ_DHKEYEXCHANGE_H
#define DSJ_DHKEYEXCHANGE_H


#include <gmpxx.h>

class DHKeyExchange {
public:
    mpz_class P, G, c, k;
    DHKeyExchange(mpz_class& P, mpz_class& G): P(P), G(G){};
    DHKeyExchange(): P(0), G(0){}
    void generateprivatekey(int len, gmp_randclass& r);
    mpz_class getpublickey();
    void received(const mpz_class& key2);
    mpz_class getkey() const;
    static mpz_class getrandomprime(int len, gmp_randclass& r);
    static mpz_class getprimitiveroot(const mpz_class& x);

    static void getb(char *dst, const mpz_class& k);

    void init(const mpz_class &P, const mpz_class &G);

    static mpz_class getmpz(char *src, int len);
};


#endif //DSJ_DHKEYEXCHANGE_H
