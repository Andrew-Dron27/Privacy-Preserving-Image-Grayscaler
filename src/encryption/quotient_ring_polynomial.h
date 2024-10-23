#ifndef QUOTIENTRINGPOLY_H
#define QUOTIENTRINGPOLY_H

#include "polynomial.h"
#include <iostream>
#include <vector>
#include <cmath>

class QuotientRingPolynomial
{
private:
    int coef_modulus;
    int degree;
    Polynomial poly_modulus;
    Polynomial coefficients;

public:
    QuotientRingPolynomial();
    QuotientRingPolynomial(int coef_modulus, Polynomial poly_modulus, Polynomial coef);

    Polynomial pad(Polynomial poly);

    void reduce();
    void round_poly(int num, int base);
    int mod_center(int num, int mod, bool left_closed = true);
    Polynomial mod_center(Polynomial poly, int mod, bool left_close = true);
    bool check_q_ring(QuotientRingPolynomial);

    QuotientRingPolynomial negate();
    QuotientRingPolynomial operator+(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator-(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator*(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator*(int scalar);
    QuotientRingPolynomial operator/(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator/(int scalar);
    QuotientRingPolynomial operator%(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator%(int modulus);
    bool operator==(const QuotientRingPolynomial& other);

    int get_coef_modulus();
    void set_coef_modulus();

    int get_degree();

    Polynomial get_poly_modulus();
    void set_poly_modulus();

    Polynomial get_coefficients();

};
#endif