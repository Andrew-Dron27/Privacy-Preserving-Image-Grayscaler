#include "encryptor.h"
Encryptor::Encryptor(){}
Encryptor::Encryptor(int ptm, Polynomial pm, int cm) : plaintext_modulus(ptm), poly_modulus(pm), coef_modulus(cm){}

Encryptor::Encryptor(int ptm, int n, int cm)
{
    std::cout << "CREATING ENCRYPTOR CLASS" << "\n";
    plaintext_modulus = ptm;
    coef_modulus = cm;
    std::cout << "CREATING POLY MODULUS" << "\n";
    poly_modulus = create_poly_modulus(n);
    std::cout << "CREATING PRIVATE KEY" << "\n";
    private_key = gen_private_key();
    std::cout << "CREATING PUBLIC KEY" << "\n";
    public_key = gen_public_key();
}

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

CipherText Encryptor::encrypt(int plain_text)
{
    QuotientRingPolynomial e1 = random_normal_poly();
    QuotientRingPolynomial e2 = random_normal_poly();
    QuotientRingPolynomial u = random_ternary_poly();
    QuotientRingPolynomial C1 = public_key.c1 * u + e1 * plaintext_modulus 
        + QuotientRingPolynomial(coef_modulus, poly_modulus, encode_message(plain_text));
    QuotientRingPolynomial C2 = public_key.c2 * u + e2 * plaintext_modulus;

    return {C1, C2};
}

int Encryptor::decrypt(CipherText cipher_text)
{
    QuotientRingPolynomial message = (cipher_text.c1 + cipher_text.c2 * private_key) % coef_modulus;
    return (decode_message(message));
}

int Encryptor::decrypt_quad(CipherTextQuad quad)
{
    QuotientRingPolynomial linear_msg = quad.c1 + quad.c2 * private_key + quad.c3 * private_key * private_key;

    //noise = np.max(np.abs(msg.coef))
    return decode_message(linear_msg % coef_modulus);
}

Polynomial Encryptor::create_poly_modulus(int n)
{
    std::vector<int> coef(n,0);
    coef[0] = 1;
    coef[n-1] = 1;
    return Polynomial(coef);
}

CipherText Encryptor::add(CipherText lhs, CipherText rhs)
{
    return CipherText(lhs.c1 + rhs.c1, lhs.c2 + rhs.c2);
}

CipherTextQuad Encryptor::mult(CipherText lhs, CipherText rhs)
{
    return CipherTextQuad(lhs.c1 * rhs.c1, lhs.c1 * rhs.c2 + lhs.c2 * rhs.c1, lhs.c2 * rhs.c2);
}

CipherText Encryptor::gen_public_key()
{
    QuotientRingPolynomial e = random_normal_poly();
    QuotientRingPolynomial a = random_uniform_poly();
    
    QuotientRingPolynomial pk1 = a * private_key + e * plaintext_modulus;
    std::cout << "CREATED PUBLIC KEY" << "\n";
    return {pk1, a.negate()};
}


QuotientRingPolynomial Encryptor::gen_private_key()
{
    return random_ternary_poly();
}

QuotientRingPolynomial Encryptor::random_binary_poly()
{
    std::cout << "Creating Random binary poly" << "\n";
    std::vector<int> res(poly_modulus.degree);

    for(size_t i = 0; i < poly_modulus.degree; i++)
    {
        res[i] = binary_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}


QuotientRingPolynomial Encryptor::random_ternary_poly()
{
    std::cout << "Creating Random ternary poly" << "\n";
    std::vector<int> res(poly_modulus.degree);

    for(size_t i = 0; i < poly_modulus.degree; i++)
    {
        res[i] = ternary_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}

QuotientRingPolynomial Encryptor::random_uniform_poly()
{
    std::cout << "Creating Random uniform poly" << "\n";

    std::uniform_int_distribution<int> uniform_distribution {0, coef_modulus-1};
    
    std::vector<int> res(poly_modulus.degree);
    for(size_t i = 0; i < poly_modulus.degree; i++)
    {
        res[i] = uniform_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}

QuotientRingPolynomial Encryptor::random_normal_poly()
{
    std::cout << "Creating Random normal poly" << "\n";
    std::vector<int> res(poly_modulus.degree);

    for(size_t i = 0; i < poly_modulus.degree; i++)
    {
        res[i] = normal_distribution(generator);
    }
    
    return QuotientRingPolynomial(coef_modulus, poly_modulus, Polynomial(res));
}