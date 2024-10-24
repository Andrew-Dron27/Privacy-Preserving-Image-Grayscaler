#include "polynomial.h"
#include <stddef.h>
#include <cmath>
#include <iostream>


Polynomial::Polynomial(){

}

Polynomial::Polynomial(const std::vector<int>& coeffs) : coefficients(coeffs) {degree = coefficients.size() - 1;}

Polynomial Polynomial::multiply(const Polynomial& other) const 
{
    size_t degree1 = coefficients.size();
    size_t degree2 = other.coefficients.size();

    // Resultant polynomial will have degree degree1 + degree2 - 1
    std::vector<int> result(degree1 + degree2 - 1, 0);

    for (size_t i = 0; i < degree1; ++i) {
        for (size_t j = 0; j < degree2; ++j) {
            result[i + j] += coefficients[i] * other.coefficients[j];
        }
    }

    return Polynomial(result);
}

bool Polynomial::operator==(const Polynomial& other ) const
{
    if(other.coefficients.size() != coefficients.size())
        return false;
    for(size_t i = 0; i < coefficients.size(); i++)
    {
        if(coefficients[i] != other.coefficients[i])
            return false;
    }

    return true;
}

Polynomial Polynomial::operator*(int scalar)
{
    for(size_t i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] /= scalar;
    }

    return *this;
}

Polynomial Polynomial::operator/(int scalar)
{
    for(size_t i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] /= scalar;
    }

    return *this;
}

Polynomial Polynomial::negate()
{
    for(size_t i = 0; i < coefficients.size(); i++){
        coefficients[i] = -coefficients[i];
    }
    return *this;
}

Polynomial Polynomial::mod(int modulus)
{
    for(size_t i = 0; i < coefficients.size(); i++){
        coefficients[i] = coefficients[i] % modulus;
    }
    return *this;
}

int Polynomial::evaluate(int x) const 
{
    int result = 0;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        result += coefficients[i] * std::pow(x, i);
    }

    return result;
}

void Polynomial::print() const
{
    std::cout << "PRINTING POLYNOMIAL OF DEGREE: " << degree << "\n";
    for(int i = 0; i < coefficients.size(); i++)
    {
        std::cout << "Polynomial Coefficients: " << coefficients[i] << "\n";
    }
}

std::pair<Polynomial, Polynomial> Polynomial::divide(const Polynomial& divisor) const 
{
    std::vector<int> quotient(coefficients.size(), 0); 
    std::vector<int> remainder = coefficients;        

    if(divisor.get_coefficients().empty() || divisor.get_coefficients().back() == 0)
    {
        throw std::invalid_argument("Divide by zero: The leading term of the divisor cannot be zero.");
    }

    int divisor_degree = divisor.coefficients.size() - 1;
    int divisor_lead_coeff = divisor.coefficients[divisor_degree];

    for (int i = remainder.size() - 1; i >= divisor_degree; --i) {
        if (remainder[i] == 0) continue; 
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

std::vector<int> Polynomial::get_coefficients() const
{
    return coefficients;
}



Polynomial Polynomial::add(const Polynomial& other) const
{
    std::vector<int> result;
    size_t max_degree = std::max(coefficients.size(), other.coefficients.size());

    result.resize(max_degree, 0); 

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

Polynomial Polynomial::subtract(const Polynomial& other) const 
{
    std::vector<int> result;
    size_t max_degree = std::max(coefficients.size(), other.coefficients.size());

    result.resize(max_degree, 0);

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


