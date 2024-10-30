#ifndef POLYNOMIAL_H
#define POLYNOMIAL
#include <vector>

class Polynomial {
private:
    std::vector<int> coefficients; // coefficients[i] is the coefficient of x^i

public:
    int degree;
    Polynomial();
    Polynomial(const std::vector<int>& coeffs);
    Polynomial add(const Polynomial& other) const;

    Polynomial operator*(int scalar);

    bool operator==(const Polynomial& other) const;

    Polynomial operator/(int scalar);

    Polynomial subtract(const Polynomial& other) const;

    Polynomial multiply(const Polynomial& other) const;

    Polynomial negate();

    Polynomial mod(int modulus);

    void print() const;

    int evaluate(int x) const;

    std::pair<Polynomial, Polynomial> divide(const Polynomial& divisor) const;

    std::vector<int> get_coefficients() const;
};

#endif
