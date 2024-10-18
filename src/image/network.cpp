#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>

bool send_image(int sock, Image& image){
    int image_deets[3] = {image.height, image.width, image.channels};

    ssize_t sentBytes = send(sock, &image.size, sizeof(image.size), 0);
    
    if (sentBytes == -1) {
        std::cout << "Error when sending image size " << strerror(errno) << "\n";
        return false;
    }
    
    sentBytes = send(sock, image_deets, sizeof(image_deets), 0);
    if (sentBytes == -1) {
        std::cout << "Error when sending image details " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "sent image details" << "\n";

    int total_bytes_sent = 0;

    while(total_bytes_sent < image.size){
        sentBytes = send(sock, image.data + total_bytes_sent, image.size - total_bytes_sent, 0);
        std::cout << "sent: " << sentBytes <<" bytes of total " << image.size << "\n";
        if(sentBytes == -1){
            std::cout << "Error when sending image data " << strerror(errno) << "\n";
            return false;
        }
        
        total_bytes_sent += sentBytes;
    }
    
    return true;
}

bool recv_image(int sock, Image& image){
    int image_deets[3];
    size_t image_size;
    ssize_t receivedBytes = recv(sock, &image_size, sizeof(image_size), 0);
   
    if (receivedBytes == -1) {
        std::cout << "Error when receiving image size " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "Received image size: " << image_size << "\n";

    receivedBytes = recv(sock, &image_deets, sizeof(image_deets), 0);

    if (receivedBytes == -1) {
        std::cout << "Error when receiving image details " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "Received image details" << "\n";

    uint8_t* data = new uint8_t[image_size];


    int total_bytes_rec = 0;
    // Receive the actual data
    while (total_bytes_rec < image_size){
        receivedBytes = recv(sock, data + total_bytes_rec, image_size - total_bytes_rec, 0);
        std::cout << "Recieved: " << receivedBytes << " bytes of total: " << image_size<< "\n";
        if(receivedBytes == -1){
            std::cout << "Error when recieving image data " << strerror(errno) << "\n";
            return false;
        }
        
        total_bytes_rec += receivedBytes;
    }

    image.data = data;
    image.size = image_size;

    image.width = image_deets[0];
    image.height = image_deets[1];
    image.channels = image_deets[2];

    return true;
}