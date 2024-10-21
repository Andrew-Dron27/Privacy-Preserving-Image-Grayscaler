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

//#endregion

Polynomial::Polynomial(const std::vector<int>& coeffs) : coefficients(coeffs) {degree = coefficients.size() - 1;}

    // Function to add two polynomials
Polynomial Polynomial::add(const Polynomial& other) const {
    std::vector<int> result;
    size_t max_degree = std::max(coefficients.size(), other.coefficients.size());

    result.resize(max_degree, 0); // Initialize result vector to the size of the larger polynomial

    // Add corresponding coefficients
    for (size_t i = 0; i < max_degree; ++i) {
        if (i < coefficients.size()) {
            result[i] += coefficients[i];
        }
        if (i < other.coefficients.size()) {
            result[i] += other.coefficients[i];
        }
    }

    return Polynomial(result);
}

// Function to subtract two polynomials
Polynomial Polynomial::subtract(const Polynomial& other) const {
    std::vector<int> result;
    size_t max_degree = std::max(coefficients.size(), other.coefficients.size());

    result.resize(max_degree, 0);

    // Subtract corresponding coefficients
    for (size_t i = 0; i < max_degree; ++i) {
        if (i < coefficients.size()) {
            result[i] += coefficients[i];
        }
        if (i < other.coefficients.size()) {
            result[i] -= other.coefficients[i];
        }
    }

    return Polynomial(result);
}

// Function to multiply two polynomials
Polynomial Polynomial::multiply(const Polynomial& other) const {
    size_t degree1 = coefficients.size();
    size_t degree2 = other.coefficients.size();

    // Resultant polynomial will have degree degree1 + degree2 - 1
    std::vector<int> result(degree1 + degree2 - 1, 0);

    // Multiply coefficients
    for (size_t i = 0; i < degree1; ++i) {
        for (size_t j = 0; j < degree2; ++j) {
            result[i + j] += coefficients[i] * other.coefficients[j];
        }
    }

    return Polynomial(result);
}

Polynomial Polynomial::negate(){
    for(size_t i = 0; i < coefficients.size(); i++){
        coefficients[i] = -coefficients[i];
    }
    return *this;
}

    // Function to evaluate the polynomial at a given value of x
int Polynomial::evaluate(int x) const {
    int result = 0;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        result += coefficients[i] * std::pow(x, i);
    }

    return result;
}

std::pair<Polynomial, Polynomial> Polynomial::divide(const Polynomial& divisor) const {
    std::vector<int> quotient(coefficients.size(), 0); // To store the quotient
    std::vector<int> remainder = coefficients;         // Start with dividend as remainder

    int divisor_degree = divisor.coefficients.size() - 1;
    int divisor_lead_coeff = divisor.coefficients[divisor_degree]; // Leading coefficient of divisor

    // Perform division
    for (int i = remainder.size() - 1; i >= divisor_degree; --i) {
        if (remainder[i] == 0) continue; // Skip if the coefficient is 0

        // Calculate the term to subtract from the remainder
        int coeff = remainder[i] / divisor_lead_coeff; // Quotient term coefficient
        int degree_diff = i - divisor_degree; // Difference in degree between dividend and divisor

        quotient[degree_diff] = coeff; // Store quotient term

        // Subtract divisor * (quotient term) from remainder
        for (int j = 0; j <= divisor_degree; ++j) {
            remainder[degree_diff + j] -= coeff * divisor.coefficients[j];
        }
    }

    // Remove leading zeros in remainder
    while (!remainder.empty() && remainder.back() == 0) {
        remainder.pop_back();
    }

    return {Polynomial(quotient), Polynomial(remainder)};
}