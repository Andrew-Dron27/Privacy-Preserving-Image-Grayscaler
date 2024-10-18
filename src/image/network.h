#ifndef NETWORK_H   // If not defined
#define NETWORK_H   // Define it

#include "image.h"

bool send_image(int sock, Image& image);

bool recv_image(int sock, Image& image);

#endif