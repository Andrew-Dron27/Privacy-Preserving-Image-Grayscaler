#ifndef IMAGE_ENCRYPTOR_H
#define IMAGE_ENCRYPTOR_H

#include <helib/helib.h>
#include <helib/binaryArith.h>
#include <helib/intraSlot.h>
#include "image.h"

struct enc_pixel_data{
    enc_pixel_data(helib::Ctxt _red, helib::Ctxt _green, helib::Ctxt _blue) : red(_red), green(_green), blue(_blue){}
    helib::Ctxt red;
    helib::Ctxt green;
    helib::Ctxt blue;
};

class ImageEncryptor
{

public:
    static std::vector<enc_pixel_data> encrypt_image_data(const Image& img, const helib::Context& context,
        helib::PubKey public_key)
    {
        //const helib::EncryptedArray& ea = context.getEA();
        std::cout << "Encrypting image data" << "\n";
        std::vector<enc_pixel_data> encrypted_data;
        helib::Ptxt<helib::BGV> red(context);
        helib::Ptxt<helib::BGV> green(context);
        helib::Ptxt<helib::BGV> blue(context);
        std::cout << "Image size" << img.size << "\n";
        for(size_t i = 0; i < img.size; i+=img.channels)
        {
            std::cout << "Encrypting: " << i << "\n";
            red[0] = img.data[i];
            green[0] = img.data[i+1];
            blue[0] = img.data[i+2];
            encrypted_data.push_back(ImageEncryptor::encrypt_pixel(red, green, blue, public_key));
        }

        return encrypted_data;
    }

    // Assumes image data is in three vectors: redChannel, greenChannel, blueChannel
    static void encrypt_image(const Image& img, const helib::PubKey& publicKey, const helib::Context& context,
                    helib::Ctxt& ctxt_red, helib::Ctxt& ctxt_green, helib::Ctxt& ctxt_blue) {
        std::vector<long> red_channel;
        std::vector<long> blue_channel; 
        std::vector<long> green_channel;

        img.get_channels(red_channel, blue_channel, green_channel);

        
        // Encode the plaintext RGB channels using the context
        helib::Ptxt<helib::BGV> ptxt_red(context, red_channel);
        helib::Ptxt<helib::BGV> ptxt_green(context, green_channel);
        helib::Ptxt<helib::BGV> ptxt_blue(context, blue_channel);

        // Encrypt each color channel separately
        publicKey.Encrypt(ctxt_red, ptxt_red);
        publicKey.Encrypt(ctxt_green, ptxt_green);
        publicKey.Encrypt(ctxt_blue, ptxt_blue);

        std::cout << "Image RGB data encrypted successfully." << std::endl;
    }

    void decryptImage(const helib::SecKey& secretKey, const helib::Context& context,
                  const helib::Ctxt& ctxt_red, const helib::Ctxt& ctxt_green, 
                  const helib::Ctxt& ctxt_blue,
                  std::vector<long>& redChannel, std::vector<long>& greenChannel, 
                  std::vector<long>& blueChannel) {

        // Decrypt each color channel ciphertext into plaintexts
        helib::Ptxt<helib::BGV> ptxt_red(context);
        helib::Ptxt<helib::BGV> ptxt_green(context);
        helib::Ptxt<helib::BGV> ptxt_blue(context);

        secretKey.Decrypt(ptxt_red, ctxt_red);
        secretKey.Decrypt(ptxt_green, ctxt_green);
        secretKey.Decrypt(ptxt_blue, ctxt_blue);

        // Extract the decrypted data from each plaintext and store in vectors
    for (size_t i = 0; i < ptxt_red.size(); i++) 
    {
        redChannel.push_back(ptxt_red[i].rep[0]);  // Directly store as `long`
    }
    for (const auto& val : ptxt_green) {
        greenChannel.push_back(val);  // Directly store as `long`
    }
    for (const auto& val : ptxt_blue) {
        blueChannel.push_back(val);  // Directly store as `long`
    }

        std::cout << "Image RGB data decrypted successfully." << std::endl;
    }

    static Image decrypt_image_data(std::vector<enc_pixel_data> encrypted_image, const helib::Context& context,
         helib::SecKey secret_key, int scale_factor, int w, int h)
    {
        std::vector<uint8_t> img_data(encrypted_image.size() * 3);

        helib::Ptxt<helib::BGV> decryptedRed(context);
        helib::Ptxt<helib::BGV> decryptedGreen(context);
        helib::Ptxt<helib::BGV> decryptedBlue(context);

        for(size_t i = 0; i < encrypted_image.size(); i++)
        {
            secret_key.Decrypt(decryptedRed, encrypted_image[i].red);
            secret_key.Decrypt(decryptedGreen, encrypted_image[i].green);
            secret_key.Decrypt(decryptedBlue, encrypted_image[i].blue);

            img_data[i] = bits_to_int(decryptedRed) / scale_factor;
            img_data[i+1] = bits_to_int(decryptedGreen) / scale_factor;
            img_data[i+2] = bits_to_int(decryptedBlue) / scale_factor;
        }

        return Image(img_data.data(), img_data.size(), w, h);
    }

    static std::vector<enc_pixel_data> greyscale_data(std::vector<enc_pixel_data> enc_data, helib::PubKey public_key, 
        helib::Ctxt scalar)
    {
        helib::Ctxt grey_pixel(public_key);

        for(size_t i = 0; i < enc_data.size(); i++)
        {
            grey_pixel += enc_data[i].red;
            grey_pixel += enc_data[i].blue;
            grey_pixel += enc_data[i].green;
            grey_pixel *= scalar;
            enc_data[i] = enc_pixel_data(grey_pixel, grey_pixel, grey_pixel);
        }
        return enc_data;
    }

    static enc_pixel_data encrypt_pixel(helib::Ptxt<helib::BGV> red, helib::Ptxt<helib::BGV> green,
        helib::Ptxt<helib::BGV> blue, helib::PubKey public_key)
    {
        helib::Ctxt r(public_key);
        helib::Ctxt g(public_key);
        helib::Ctxt b(public_key);
        public_key.Encrypt(r, red);
        public_key.Encrypt(g, green);
        public_key.Encrypt(b, blue);

        return enc_pixel_data(r, g, b);
    }
    

    static int bits_to_int(helib::Ptxt<helib::BGV> bits)
    {
        int message = 0;

        // Iterate over the vector, assuming bits are in reverse order (LSB to MSB)
        for (size_t i = 0; i < bits.size(); ++i) {
            if (bits[i] == 1) {
                message |= (1 << i);
            }
        }

        return message;
    }

    static helib::Ptxt<helib::BGV> int_to_bits(int num)
    {
        helib::Ptxt<helib::BGV> bits;
        for (size_t i = 0; i < sizeof(int) * 8; ++i) {
            bits[i] = (num >> i) & 1;
        }
        return bits;
    }

    static const helib::Context& init_helib_context()
    {
        std::cout << "Initializing helib context" << "\n";
        // Plaintext prime modulus.
        long p = 2;
        // Cyclotomic polynomial - defines phi(m).
        long m = 4095;
        // Hensel lifting (default = 1).
        long r = 1;
        // Number of bits of the modulus chain.
        long bits = 500;
        // Number of columns of Key-Switching matrix (typically 2 or 3).
        long c = 2;
        // Factorisation of m required for bootstrapping.
        std::vector<long> mvec = {7, 5, 9, 13};
        // Generating set of Zm* group.
        std::vector<long> gens = {2341, 3277, 911};
        // Orders of the previous generators.
        std::vector<long> ords = {6, 4, 6};

        const helib::Context& context = helib::ContextBuilder<helib::BGV>()
                                .m(m)
                                .p(p)
                                .r(r)
                                .gens(gens)
                                .ords(ords)
                                .bits(bits)
                                .c(c)
                                .bootstrappable(true)
                                .mvec(mvec)
                                .build();
        //context.printout(); 
        return context;
    }
};

#endif