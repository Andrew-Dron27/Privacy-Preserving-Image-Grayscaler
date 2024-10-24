#ifndef POLYNOMIAL_H
#define POLYNOMIAL
#include <vector>

class Polynomial {
private:
    std::vector<int> coefficients; // coefficients[i] is the coefficient of x^i

public:
    int degree;
    // Constructor to initialize a polynomial with given coefficients
    Polynomial();
    Polynomial(const std::vector<int>& coeffs);

    // Function to add two polynomials
    Polynomial add(const Polynomial& other) const;

    Polynomial operator*(int scalar);

    bool operator==(const Polynomial& other) const;

    Polynomial operator/(int scalar);

    // Function to subtract two polynomials
    Polynomial subtract(const Polynomial& other) const;

    // Function to multiply two polynomials
    Polynomial multiply(const Polynomial& other) const;

    Polynomial negate();

    Polynomial mod(int modulus);

    void print() const;

    // Function to evaluate the polynomial at a given value of x
    int evaluate(int x) const;

    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;

    std::vector<int> get_coefficients() const;
};

#endif
