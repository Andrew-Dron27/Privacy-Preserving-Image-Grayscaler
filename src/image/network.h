#ifndef NETWORK_H   // If not defined
#define NETWORK_H   // Define it

#include "image.h"
#include "image_encryptor.h"

bool send_image(int sock, Image& image);

bool recv_image(int sock, Image& image);

bool send_encrypted_image(int sock, std::vector<enc_pixel_data>, 
    helib::Ctxt scalar, const helib::PubKey& public_key);

bool recv_encrypted_image(int sock, Image& image, int w, int h, int scale_factor,
    const helib::Context& context, const helib::SecKey& secret_key);

#endif