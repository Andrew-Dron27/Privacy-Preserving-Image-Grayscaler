#include "encryption.h"

QuotientRingPolynomial::QuotientRingPolynomial(int _coef_modulus, Polynomial _poly_modulus, Polynomial _coef) 
    : coef_modulus(_coef_modulus), poly_modulus(_poly_modulus), coefficients(_coef)
{
    degree = poly_modulus.degree;
    reduce();
}

void QuotientRingPolynomial::reduce()
{

}

Polynomial QuotientRingPolynomial::init_poly_modulus(int modulus)
{
    std::vector<int> poly;
    poly_modulus = Polynomial(poly);
}

Polynomial QuotientRingPolynomial::init_poly_modulus(Polynomial modulus)
{
    poly_modulus = modulus;
}

Polynomial QuotientRingPolynomial::pad(Polynomial poly)
{
    
}


void QuotientRingPolynomial::round_poly(int num, int base)
{

}

int QuotientRingPolynomial::mod_center(int num, int mod, bool left_closed = true)
{

}

bool QuotientRingPolynomial::check_q_ring(QuotientRingPolynomial)
{

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
    return QuotientRingPolynomial(coef_modulus, result, poly_modulus); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator-(const QuotientRingPolynomial& other)
{
    if(!check_q_ring(other))
    {
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.subtract(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, result, poly_modulus); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator*(const QuotientRingPolynomial& other)
{
    if(!check_q_ring(other))
    {
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.multiply(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, result, poly_modulus); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator%(const QuotientRingPolynomial& other)
{

    //TODO: FIX THIS
    if(!check_q_ring(other)){
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.multiply(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, result, poly_modulus); 
}

QuotientRingPolynomial QuotientRingPolynomial::operator=(const QuotientRingPolynomial& other)
{
    //TODO: FIX THIS
    if(!check_q_ring(other)){
        std::cout << "Polynomails are not in the same ring! ABORT! ABORT BY GOD!" << "\n";
    }
    Polynomial result = coefficients.multiply(other.coefficients);
    return QuotientRingPolynomial(coef_modulus, result, poly_modulus); 
}