#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <iostream>
#include <vector>
#include <cmath>
#include "quotient_ring_polynomial.h"

class Encryptor
{
private:
    std::pair<QuotientRingPolynomial, QuotientRingPolynomial> public_key;
    std::pair<QuotientRingPolynomial, QuotientRingPolynomial> private_key;

    //polynomial modulus degree
    int n;

    //plain text modulus;
    QuotientRingPolynomial t;
    //cipher text modulus
    QuotientRingPolynomial q;

    //TODO: Find out how to generate noise distribution
    int x;

public:
    QuotientRingPolynomial encode_message(int message);
    std::pair<QuotientRingPolynomial, QuotientRingPolynomial> encrypt(int plain_text);
    int decrypt(std::pair<QuotientRingPolynomial, QuotientRingPolynomial> cipher_text);


    int add(std::pair<QuotientRingPolynomial, QuotientRingPolynomial> c1,
         std::pair<QuotientRingPolynomial, QuotientRingPolynomial> c2);
    int mult(std::pair<QuotientRingPolynomial, QuotientRingPolynomial> c1,
         std::pair<QuotientRingPolynomial, QuotientRingPolynomial> c2);

    std::pair<QuotientRingPolynomial, QuotientRingPolynomial> gen_secret_key(int coef_modulus, int poly_modulus);
    std::pair<QuotientRingPolynomial, QuotientRingPolynomial> gen_public_key(QuotientRingPolynomial sk, int coef_modulus,
        Polynomial poly_modulus, int plaintext_modulus);

    QuotientRingPolynomial random_binary_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_ternary_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_uniform_poly(int coef_modulus, Polynomial poly_modulus);
    QuotientRingPolynomial random_normal_poly(int coef_modulus, Polynomial poly_modulus);
};
#endif