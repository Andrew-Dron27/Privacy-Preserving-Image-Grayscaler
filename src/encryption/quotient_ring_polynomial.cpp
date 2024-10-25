#include "quotient_ring_polynomial.h"

#pragma region "Quotient Ring"

//#region

QuotientRingPolynomial::QuotientRingPolynomial(int _coef_modulus, Polynomial _poly_modulus, Polynomial _coef) 
    : coef_modulus(_coef_modulus), poly_modulus(_poly_modulus), coefficients(_coef)
{
    degree = poly_modulus.degree;
    reduce();
}

void QuotientRingPolynomial::reduce()
{
    coefficients = mod_center(coefficients, coef_modulus);
    coefficients = coefficients.divide(poly_modulus).second;
    coefficients = mod_center(coefficients, coef_modulus);
    coefficients = pad(coefficients);
}

Polynomial QuotientRingPolynomial::pad(Polynomial poly)
{
    //std::cout << "PADDING: ";
    //poly.print();
    std::vector<int> coef = poly.get_coefficients();
    if(coef.size() >= degree)
        return poly;
    std::vector<int> padding(degree - poly.get_coefficients().size() + 1, 0);
    coef.insert(coef.end(), padding.begin(), padding.end());
   // Polynomial(coef).print();
    return Polynomial(coef);
}

Polynomial QuotientRingPolynomial::mod_center(Polynomial poly, int mod, bool left_close)
{
    auto coef = poly.get_coefficients();
    for(auto& i : coef)
    {
        i = mod_center(i, mod, left_close);
    }
    return coef;
}

int QuotientRingPolynomial::mod_center(int num, int mod, bool left_closed)
{
    if(left_closed)
    {
        return (num + mod / 2) % mod - mod / 2;
    }
    else
    {
        return (num + mod / 2 - 1) % mod - mod / 2 + 1;
    }
}

bool QuotientRingPolynomial::check_q_ring(QuotientRingPolynomial)
{
    //TODO: FIX THIS
    return true;
}


QuotientRingPolynomial QuotientRingPolynomial::negate()
{
    return QuotientRingPolynomial(coef_modulus, poly_modulus, coefficients.negate());
}


QuotientRingPolynomial QuotientRingPolynomial::operator+(const QuotientRingPolynomial& other)
{
    if(!check_q_ring(other))
    {
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.add(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator-(const QuotientRingPolynomial& other)
{
    if(!check_q_ring(other))
    {
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.subtract(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator*(const QuotientRingPolynomial& other)
{
    if(!check_q_ring(other))
    {
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.multiply(other.coefficients);
    result.print();
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator*(int scalar)
{
    Polynomial result = coefficients * scalar;

    return QuotientRingPolynomial(coef_modulus, poly_modulus, result); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator/(const QuotientRingPolynomial& other)
{
    std::pair<Polynomial, Polynomial> result = coefficients.divide(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result.first);
}

QuotientRingPolynomial QuotientRingPolynomial::operator/(int scalar)
{
    Polynomial result = coefficients / scalar;
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator%(const QuotientRingPolynomial& other)
{
    std::pair<Polynomial, Polynomial> result = coefficients.divide(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, poly_modulus, result.second);
}

QuotientRingPolynomial QuotientRingPolynomial::operator%(int modulus)
{
    coefficients.mod(modulus);
    return *this;
}

bool QuotientRingPolynomial::operator==(const QuotientRingPolynomial& other)
{
    return (coef_modulus == other.coef_modulus, coefficients == other.coefficients && poly_modulus == other.poly_modulus);
}

Polynomial QuotientRingPolynomial::get_coefficients()
{
    return coefficients;
}

void QuotientRingPolynomial::print()
{
    std::cout << "Coef Modulus: " << coef_modulus << " Poly_Modulus:\n ";
    poly_modulus.print();
    std::cout << "Coefficients:\n ";
    coefficients.print();
}

//#endregion
