#ifndef NETWORK_H   // If not defined
#define NETWORK_H   // Define it

#include "image.h"
#include "image_encryptor.h"
#include <helib/helib.h>

bool send_image(int sock, Image& image);

bool recv_image(int sock, Image& image);

bool send_encrypted_image(int sock, const std::vector<enc_pixel_data>&, 
    const helib::PubKey& public_key);

bool send_public_key(int sock, const helib::PubKey& publicKey);

helib::PubKey receive_public_key(int sock, const helib::Context& context);

bool send_cipher_texts(int sock, std::vector<enc_pixel_data> enc_pixels);

std::vector<enc_pixel_data> recv_encrypted_pixels(int sock, const helib::PubKey& public_key);

std::vector<enc_pixel_data> recv_encrypted_pixels(int sock, const helib::PubKey& public_key);

bool recv_image(int sock, Image& image);

#endif