#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <iostream>
#include <vector>
#include <cmath>
#include "quotient_ring_polynomial.h"
#include <random>

struct CipherText {
    QuotientRingPolynomial c1;
    QuotientRingPolynomial c2;

    CipherText();
    CipherText(QuotientRingPolynomial _c1, QuotientRingPolynomial _c2) : c1(_c1), c2(_c2){}
};

struct CipherTextQuad {
    QuotientRingPolynomial c1;
    QuotientRingPolynomial c2;
    QuotientRingPolynomial c3;

    CipherTextQuad();
    CipherTextQuad(QuotientRingPolynomial _c1, QuotientRingPolynomial _c2, QuotientRingPolynomial _c3)
     : c1(_c1), c2(_c2), c3(_c3){}
};

class Encryptor
{
private:
    std::default_random_engine generator;

    std::uniform_int_distribution<int> binary_distribution {0,1};
    std::uniform_int_distribution<int> ternary_distribution {-1,1};
    std::normal_distribution<double> normal_distribution {0, 3.8};

    CipherText public_key;
    QuotientRingPolynomial private_key;

    //polynomial modulus degree
    int n;

    //plain text modulus;
    QuotientRingPolynomial t;
    //cipher text modulus
    QuotientRingPolynomial q;

    //TODO: Find out how to generate noise distribution
    int x;

public:
    Polynomial encode_message(int message);
    int decode_message(QuotientRingPolynomial encoded);

    CipherText encrypt(int plain_text,
     int coef_modulus, Polynomial poly_modulus, int plain_text_modulus);
     
    int decrypt(CipherText cipher_text,
        int coef_modulus, Polynomial poly_modulus);

    int decrypt_quad(CipherTextQuad quad, int coef_modulus);


    CipherText add(CipherText lhs,
         CipherText rhs);
    CipherTextQuad mult(CipherText lhs,
         CipherText rhs);

    CipherText relinearize(CipherTextQuad quad);

    QuotientRingPolynomial gen_private_key(int coef_modulus, Polynomial poly_modulus);
    CipherText gen_public_key(QuotientRingPolynomial sk, int coef_modulus,
        Polynomial poly_modulus, int plaintext_modulus);

    QuotientRingPolynomial random_binary_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_ternary_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_uniform_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_normal_poly(int coef_modulus, Polynomial poly_modulus);
};
#endif