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

bool send_encrypted_image(int sock, const std::vector<enc_pixel_data>& enc_data,
     helib::Ctxt scalar, const helib::PubKey& public_key)
{

    //send public key
    size_t pb_size = sizeof(public_key);

    ssize_t sentBytes = send(sock, &pb_size, sizeof(pb_size), 0);
    
    if (sentBytes == -1) {
        std::cout << "Error when sending public key size " << strerror(errno) << "\n";
        return false;
    }

    sentBytes = send(sock, &public_key, pb_size, 0);

    if (sentBytes == -1) {
        std::cout << "Error when sending public key " << strerror(errno) << "\n";
        return false;
    }

    int size = enc_data.size();

    ssize_t sentBytes = send(sock, &size, sizeof(enc_data.size()), 0);
    
    if (sentBytes == -1) {
        std::cout << "Error when sending encrypted image size " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "sent image details" << "\n";

    int total_bytes_sent = 0;

    while(total_bytes_sent < size){
        sentBytes = send(sock, &enc_data + total_bytes_sent, size - total_bytes_sent, 0);
        std::cout << "sent: " << sentBytes <<" bytes of total " << size << "\n";
        if(sentBytes == -1){
            std::cout << "Error when sending image data " << strerror(errno) << "\n";
            return false;
        }
        
        total_bytes_sent += sentBytes;
    }
    
    return true;
}


bool recv_encrypted_image(int sock, Image& image, int width, int height, 
    int scale_factor, const helib::Context& context, const helib::SecKey& secret_key)
{
    std::vector<enc_pixel_data> enc_data;

    helib::PubKey public_key = receivePublicKey(sock, context);

    std::vector<enc_pixel_data> enc_pixels = recv_encrypted_pixels(sock, public_key);

    image = ImageEncryptor::decrypt_image_data(enc_data, context, secret_key, scale_factor, width, height);

    return true;
}


void sendPublicKey(const helib::PubKey& publicKey) {
    // Create a string stream to hold the serialized public key
    std::ostringstream oss;
    publicKey.writeTo(oss); // Serialize the public key into the stream

    // Retrieve the serialized data as a string
    std::string serializedData = oss.str();

    // Send `serializedData` over your chosen communication channel
    // For example, you could write this data to a file or a socket
    std::ofstream file("publicKey.bin", std::ios::binary);
    file.write(serializedData.data(), serializedData.size());
    file.close();
}


helib::PubKey receivePublicKey(int sock, const helib::Context& context) {

    size_t pb_size;

    ssize_t receivedBytes = recv(sock, &pb_size, sizeof(pb_size), 0);

    if (receivedBytes == -1) {
        std::cout << "Error when receiving public key size " << strerror(errno) << "\n";
        return;
    }

    std::string data(pb_size, '\0');
    size_t bytesReceived = 0;

    while (bytesReceived < pb_size) {
        ssize_t result = recv(sock, &data[bytesReceived], pb_size - bytesReceived, 0);
        if (result <= 0) {
            std::cout << "Error when receiving public key " << strerror(errno) << "\n";
            return;
        }
        bytesReceived += result;
    }

    
    std::stringstream oss;
    oss << data;

    // Deserialize the public key from the received data
    helib::PubKey publicKey(context);
    publicKey.readFrom(oss, context); // Deserialize into a PubKey object

    return publicKey;
}


bool send_cipher_texts(int sock, std::vector<enc_pixel_data> enc_pixels)
{
    uint32_t num_pixels = htonl(enc_pixels.size() * 3);
    if (send(sock, &num_pixels, sizeof(num_pixels), 0) == -1) {
        perror("send");
        return false;
    }

    std::vector<helib::Ctxt> cipher_texts(num_pixels);
    // Serialize and send each ciphertext
    for (const auto& pixel : enc_pixels) 
    {
        cipher_texts.push_back(pixel.red);
        cipher_texts.push_back(pixel.green);
        cipher_texts.push_back(pixel.blue);
    }

    // Serialize and send each ciphertext
    for (const auto& ciphertext : cipher_texts) {
        std::ostringstream oss;
        ciphertext.writeTo(oss); // Serialize the ciphertext
        std::string serializedData = oss.str();

        // Send the size of the serialized data
        uint32_t dataSize = htonl(serializedData.size());
        if (send(sock, &dataSize, sizeof(dataSize), 0) == -1) {
            perror("send");
            return false;
        }

        // Send the actual serialized data
        if (send(sock, serializedData.c_str(), serializedData.size(), 0) == -1) {
            perror("send");
            return false;
        }
    }

    std::cout << "Succesfully sent cipher texts" << "\n";
    return true;

}

// bool send_cipher_text(int sock, const helib::Ctxt cipher_text)
// {
//     std::ostringstream oss;
//     cipher_text.writeTo(oss); // Serialize the ciphertext
//     std::string serializedData = oss.str();
//     // Send the size of the serialized data
//     uint32_t dataSize = htonl(serializedData.size());
//     if (send(sock, &dataSize, sizeof(dataSize), 0) == -1) 
//     {
//         perror("send");
//         return;
//     }

//     // Send the actual serialized data
//     if (send(sock, serializedData.c_str(), serializedData.size(), 0) == -1)
//     {
//         perror("send");
//         return;
    
//     }
// }

std::vector<enc_pixel_data> recv_encrypted_pixels(int sock, const helib::PubKey& public_key)
{
    // Receive the number of ciphertexts
    uint32_t num_ciphertexts;
    if (recv(sock, &num_ciphertexts, sizeof(num_ciphertexts), 0) == -1) {
        perror("recv");
        throw std::runtime_error("Failed to receive number of ciphertexts");
    }

    std::vector<helib::Ctxt> ciphertexts;
    ciphertexts.reserve(num_ciphertexts); // Reserve space

    // Receive each ciphertext
    for (uint32_t i = 0; i < num_ciphertexts; ++i) {
        // Receive the size of the incoming ciphertext
        uint32_t dataSize;
        if (recv(sock, &dataSize, sizeof(dataSize), 0) == -1) {
            perror("recv");
            throw std::runtime_error("Failed to receive data size");
        }
        dataSize = ntohl(dataSize);

        // Receive the actual serialized ciphertext
        std::string serializedData(dataSize, '\0');
        size_t bytesReceived = 0;
        while (bytesReceived < dataSize) {
            ssize_t result = recv(sock, &serializedData[bytesReceived], dataSize - bytesReceived, 0);
            if (result <= 0) {
                perror("recv");
                throw std::runtime_error("Failed to receive data");
            }
            bytesReceived += result;
        }

        // Deserialize the ciphertext
        std::istringstream iss(serializedData);
        helib::Ctxt ciphertext(public_key);
        ciphertext.read(iss); // Deserialize from the stream
        ciphertexts.push_back(std::move(ciphertext));
    }

    std::vector<enc_pixel_data> enc_pixels;

    for(int i = 0; i < ciphertexts.size(); i+=3)
    {
        enc_pixels.push_back(enc_pixel_data(ciphertexts[i], ciphertexts[i+1], ciphertexts[i+2]));
    }
    return enc_pixels;
}

// void recv_encrypted_data(int sock)
// {
//     size_t data_size;

//     size_t receivedBytes = recv(sock, &data_size, sizeof(data_size), 0);
   
//     if (receivedBytes == -1) {
//         std::cout << "Error when receiving image size " << strerror(errno) << "\n";
//         return;
//     }

//     std::cout << "Received image size: " << data_size << "\n";

//     void* data = new char[data_size];// = new void[data_size];

//     int total_bytes_rec = 0;
//     // Receive the actual data
//     while (total_bytes_rec < data_size){
//         receivedBytes = recv(sock, data + total_bytes_rec, data_size - total_bytes_rec, 0);
//         std::cout << "Recieved: " << receivedBytes << " bytes of total: " << data_size << "\n";
//         if(receivedBytes == -1){
//             std::cout << "Error when recieving image data " << strerror(errno) << "\n";
//             return false;
//         }
        
//         total_bytes_rec += receivedBytes;
//     }

//     enc_pixel_data* pixel_data = static_cast<enc_pixel_data*>(data);

//     enc_data = std::vector<enc_pixel_data>(pixel_data, pixel_data + total_bytes_rec);
// }


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