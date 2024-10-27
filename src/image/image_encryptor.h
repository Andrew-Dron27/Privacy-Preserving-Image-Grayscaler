#ifndef IMAGE_ENCRYPTOR_H
#define IMAGE_ENCRYPTOR_H

#include <helib/helib.h>
#include <helib/binaryArith.h>
#include <helib/intraSlot.h>
#include "image.h"

struct enc_pixel_data{
    enc_pixel_data(helib::Ctxt _red, helib::Ctxt _green, helib::Ctxt _blue) : red(_red), blue(_blue), green(_green){}
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
        std::vector<enc_pixel_data> encrypted_data;
        int red,blue,green;
        for(int i = 0; i < img.size; i+=img.channels)
        {
            red = img.data[i];
            green = img.data[i+1];
            blue = img.data[i+2];
            encrypted_data.push_back(ImageEncryptor::encrypt_pixel(red, blue, green, context, public_key));
        }

        return encrypted_data;
    }

    static Image decrypt_image_data(std::vector<enc_pixel_data> encrypted_image, const helib::Context& context,
         helib::SecKey secret_key, int scale_factor, int w, int h)
    {
        std::vector<size_t> img_data(encrypted_image.size() * 3);

        helib::Ptxt<helib::BGV> decryptedRed(context);
        helib::Ptxt<helib::BGV> decryptedGreen(context);
        helib::Ptxt<helib::BGV> decryptedBlue(context);

        for(int i = 0; i < encrypted_image.size(); i++)
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

        for(int i = 0; i < enc_data.size(); i++)
        {
            grey_pixel += enc_data[i].red;
            grey_pixel += enc_data[i].blue;
            grey_pixel += enc_data[i].green;
            grey_pixel *= scalar;
            enc_data[i] = enc_pixel_data(grey_pixel, grey_pixel, grey_pixel);
        }
    }

    static enc_pixel_data encrypt_pixel(int red, int green, int blue, const helib::Context& context,
         helib::PubKey public_key)
    {
        helib::Ctxt r(public_key);
        helib::Ctxt g(public_key);
        helib::Ctxt b(public_key);
        public_key.Encrypt(r, ImageEncryptor::int_to_bits(red));
        public_key.Encrypt(g, ImageEncryptor::int_to_bits(green));
        public_key.Encrypt(b, ImageEncryptor::int_to_bits(blue));

        return enc_pixel_data(r, g, b);
    }
    

    static int bits_to_int(helib::Ptxt<helib::BGV> bits)
    {
        int message = 0;

        // Iterate over the vector, assuming bits are in reverse order (LSB to MSB)
        for (int i = 0; i < bits.size(); ++i) {
            if (bits[i] == 1) {
                message |= (1 << i);
            }
        }

        return message;
    }

    static helib::Ptxt<helib::BGV> int_to_bits(int num)
    {
        helib::Ptxt<helib::BGV> bits;
        for (int i = 0; i < sizeof(int) * 8; ++i) {
            bits[i] = (num >> i) & 1;
        }
        return bits;
    }
};

#endif