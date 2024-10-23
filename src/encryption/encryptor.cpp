#include "encryptor.h"

Polynomial Encryptor::encode_message(int message)
{
    std::vector<int> bits;

    // Iterate over each bit of the integer
    for (int i = 0; i < sizeof(int) * 8; ++i) {
        // Extract the least significant bit and push it to the vector
        bits.push_back((message >> i) & 1);
    }

    return Polynomial(bits);
}

int Encryptor::decode_message(QuotientRingPolynomial m)
{
    std::vector<int> bits = m.get_coefficients().get_coefficients();
    int message = 0;

    // Iterate over the vector, assuming bits are in reverse order (LSB to MSB)
    for (int i = 0; i < bits.size(); ++i) {
        if (bits[i] == 1) {
            message |= (1 << i);  // Set the i-th bit of the integer
        }
    }

    return message;
}

CipherText Encryptor::encrypt(int plain_text,
     int coef_modulus, Polynomial poly_modulus, int plain_text_modulus)
{
    QuotientRingPolynomial e1 = random_normal_poly(coef_modulus, poly_modulus);
    QuotientRingPolynomial e2 = random_normal_poly(coef_modulus, poly_modulus);
    QuotientRingPolynomial u = random_ternary_poly(coef_modulus, poly_modulus);
    QuotientRingPolynomial C1 = public_key.c1 * u + e1 * plain_text_modulus 
        + QuotientRingPolynomial(coef_modulus, encode_message(plain_text), poly_modulus);
    QuotientRingPolynomial C2 = public_key.c2 * u + e2 * plain_text_modulus;

    return {C1, C2};
}

int Encryptor::decrypt(CipherText cipher_text,
    int coef_modulus, Polynomial poly_modulus)
{
    QuotientRingPolynomial message = (cipher_text.c1 + cipher_text.c2 * private_key) % coef_modulus;
    return (decode_message(message));
}

int Encryptor::decrypt_quad(CipherTextQuad quad, int coef_modulus)
{
    QuotientRingPolynomial linear_msg = quad.c1 + quad.c2 * private_key + quad.c3 * private_key * private_key;

    //noise = np.max(np.abs(msg.coef))
    return decode_message(linear_msg % coef_modulus);
}

CipherText Encryptor::add(CipherText lhs, CipherText rhs)
{
    return CipherText(lhs.c1 + rhs.c1, lhs.c2 + rhs.c2);
}

CipherTextQuad Encryptor::mult(CipherText lhs, CipherText rhs)
{
    return CipherTextQuad(lhs.c1 * rhs.c1, lhs.c1 * rhs.c2 + lhs.c2 * rhs.c1, lhs.c2 * rhs.c2);
}



CipherText Encryptor::gen_public_key(QuotientRingPolynomial sk, int coef_modulus,
    Polynomial poly_modulus, int plaintext_modulus)
{
    QuotientRingPolynomial e = random_normal_poly(coef_modulus, poly_modulus);
    QuotientRingPolynomial a = random_uniform_poly(coef_modulus, poly_modulus);

    QuotientRingPolynomial pk1 = a * private_key + e * plaintext_modulus;
    return {pk1, a.negate()};
}


QuotientRingPolynomial Encryptor::gen_private_key(int coef_modulus, Polynomial poly_modulus)
{
    return random_ternary_poly(coef_modulus, poly_modulus);
}

QuotientRingPolynomial Encryptor::random_binary_poly(int coef_modulus, Polynomial poly_modulus)
{
    std::vector<int> res(coef_modulus - 1);

    for(size_t i = 0; i < coef_modulus - 1; coef_modulus++)
    {
        res[i] = binary_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}


QuotientRingPolynomial Encryptor::random_ternary_poly(int coef_modulus, Polynomial poly_modulus)
{
    std::vector<int> res(coef_modulus - 1);

    for(size_t i = 0; i < coef_modulus - 1; coef_modulus++)
    {
        res[i] = ternary_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}

QuotientRingPolynomial Encryptor::random_uniform_poly(int coef_modulus, Polynomial poly_modulus)
{
    std::uniform_int_distribution<int> uniform_distribution {0, coef_modulus-1};
    std::vector<int> res(coef_modulus - 1);

    for(size_t i = 0; i < coef_modulus - 1; coef_modulus++)
    {
        res[i] = uniform_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}

QuotientRingPolynomial Encryptor::random_normal_poly(int coef_modulus, Polynomial poly_modulus)
{
    std::vector<int> res(coef_modulus - 1);

    for(size_t i = 0; i < coef_modulus - 1; coef_modulus++)
    {
        res[i] = normal_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}