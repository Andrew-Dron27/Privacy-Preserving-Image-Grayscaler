#ifndef QUOTIENTRINGPOLY_H
#define QUOTIENTRINGPOLY_H

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

    Polynomial init_poly_modulus(int modulus);
    Polynomial init_poly_modulus(Polynomial modulus);
    Polynomial pad(Polynomial poly);

    void reduce();
    void round_poly(int num, int base);
    int mod_center(int num, int mod, bool left_closed = true);
    bool check_q_ring(QuotientRingPolynomial);
    QuotientRingPolynomial negate();
    QuotientRingPolynomial floor_div();
 
    QuotientRingPolynomial operator+(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator-(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator*(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator%(const QuotientRingPolynomial& other);
    QuotientRingPolynomial operator=(const QuotientRingPolynomial& other);

    int get_coef_modulus();
    void set_coef_modulus();

    int get_degree();

    Polynomial get_poly_modulus();
    void set_poly_modulus();

    Polynomial get_coefficients();
    void set_poly_modulus();

};


class Polynomial {
private:
    std::vector<int> coefficients; // coefficients[i] is the coefficient of x^i

public:
    int degree;
    // Constructor to initialize a polynomial with given coefficients
    Polynomial(const std::vector<int>& coeffs);

    // Function to add two polynomials
    Polynomial add(const Polynomial& other) const;

    // Function to subtract two polynomials
    Polynomial subtract(const Polynomial& other) const;

    // Function to multiply two polynomials
    Polynomial multiply(const Polynomial& other) const;

    Polynomial negate();

    // Function to evaluate the polynomial at a given value of x
    int evaluate(int x) const;

    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;
};

#endif