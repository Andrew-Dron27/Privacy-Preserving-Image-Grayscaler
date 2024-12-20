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

    size_t total_bytes_sent = 0;

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

bool send_encrypted_image(int sock, const std::vector<enc_pixel_data>& enc_data)
{
    size_t data_size = enc_data.size();

    std::cout << "Sending Encrypted Image" << "\n";

    ssize_t sentBytes = send(sock, &data_size, sizeof(enc_data.size()), 0);
    
    if (sentBytes == -1) {
        std::cout << "Error when sending encrypted image size " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "sent image details" << "\n";

    size_t total_bytes_sent = 0;

    while(total_bytes_sent < data_size){
        sentBytes = send(sock, &enc_data + total_bytes_sent, data_size - total_bytes_sent, 0);
        std::cout << "sent: " << sentBytes <<" bytes of total " << data_size << "\n";
        if(sentBytes == -1){
            std::cout << "Error when sending image data " << strerror(errno) << "\n";
            return false;
        }
        
        total_bytes_sent += sentBytes;
    }
    
    return true;
}


// std::vector<enc_pixel_data> recv_encrypted_image(int sock, const helib::Context& context, helib::PubKey public_key)
// {
//     std::vector<enc_pixel_data> enc_pixels = recv_encrypted_pixels(sock, public_key);

//     //image = ImageEncryptor::decrypt_image_data(enc_data, context, secret_key, scale_factor, width, height);

//     return enc_pixels;
// }


bool send_public_key(int sock, const helib::PubKey& public_key) 
{
    std::cout << "Sending Public key to server" << "\n";
    // Create a string stream to hold the serialized public key
    std::ostringstream oss;
    public_key.writeTo(oss); // Serialize the public key into the stream

    //std::cout << "Sending public key to server: " << oss.str() << "\n";

    // Retrieve the serialized data as a string
    std::string serializedData = oss.str();

    //send public key
    size_t pb_size = sizeof(serializedData);

    ssize_t sentBytes = send(sock, &pb_size, sizeof(pb_size), 0);
    
    if (sentBytes == -1) {
        std::cout << "Error when sending public key size " << strerror(errno) << "\n";
        return false;
    }

    sentBytes = send(sock, &serializedData, pb_size, 0);

    if (sentBytes == -1) {
        std::cout << "Error when sending public key " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "Succesfully sent public key to server" << "\n";
    return true;
}


helib::PubKey receive_public_key(int sock, const helib::Context& context) 
{

    std::cout << "Recveiving public key from client" << "\n";
    size_t pb_size;

    ssize_t receivedBytes = recv(sock, &pb_size, sizeof(pb_size), 0);

    if (receivedBytes == -1)
     {
        std::cout << "Error when receiving public key size " << strerror(errno) << "\n";

        //perror("send");
        throw std::runtime_error("");
    }

    std::cout << "Recveived public key size: " << receivedBytes << "\n";

    //std::string data(pb_size, '\0');
    char* data[pb_size];
    size_t bytesReceived = 0;

    while (bytesReceived < pb_size) 
    {
        ssize_t result = recv(sock, data + bytesReceived, pb_size - bytesReceived, 0);
        if (result == -1) 
        {
            std::cout << "Error when receiving public key " << strerror(errno) << "\n";
            throw std::runtime_error("");
        }
        bytesReceived += result;
    }

    std::cout << "Recveived public key data" << "\n";
    
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
    if (send(sock, &num_pixels, sizeof(num_pixels), 0) == -1)
    {
        perror("send");
        return false;
    }

    std::vector<helib::Ctxt> cipher_texts;
    cipher_texts.reserve(num_pixels); 

    for (const auto& pixel : enc_pixels) 
    {
        cipher_texts.push_back(pixel.red);
        cipher_texts.push_back(pixel.green);
        cipher_texts.push_back(pixel.blue);
    }

    for (const auto& ciphertext : cipher_texts) 
    {
        std::ostringstream oss;
        ciphertext.writeTo(oss); 
        std::string serializedData = oss.str();

        uint32_t dataSize = htonl(serializedData.size());
        if (send(sock, &dataSize, sizeof(dataSize), 0) == -1) 
        {
            perror("send");
            return false;
        }

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
    std::cout << "Recveiving Encrypted image" << "\n";

    // Receive the number of ciphertexts
    uint32_t num_ciphertexts;
    if (recv(sock, &num_ciphertexts, sizeof(num_ciphertexts), 0) == -1) {
        perror("recv");
        throw std::runtime_error("Failed to receive total encrypted image size");
    }

    std::vector<helib::Ctxt> ciphertexts;
    ciphertexts.reserve(num_ciphertexts); 

    // Receive each ciphertext
    for (uint32_t i = 0; i < num_ciphertexts; ++i) {
        // Receive the size of the incoming ciphertext
        uint32_t dataSize;
        if (recv(sock, &dataSize, sizeof(dataSize), 0) == -1) {
            perror("recv");
            throw std::runtime_error("Failed to receive cipher text data size");
        }
        dataSize = ntohl(dataSize);

        // Receive the actual serialized ciphertext
        std::string serializedData(dataSize, '\0');
        size_t bytesReceived = 0;
        while (bytesReceived < dataSize) {
            ssize_t result = recv(sock, &serializedData[bytesReceived], dataSize - bytesReceived, 0);
            if (result <= 0) {
                perror("recv");
                throw std::runtime_error("Failed to receive cipher text");
            }
            bytesReceived += result;
        }

        // Deserialize the ciphertext
        std::istringstream iss(serializedData);
        helib::Ctxt ciphertext(public_key);
        ciphertext.read(iss); // Deserialize from the stream
        ciphertexts.push_back(std::move(ciphertext));
    }

    std::cout << "Recveived encrypted cipher texts from client" << "\n";

    std::vector<enc_pixel_data> enc_pixels;

    for(size_t i = 0; i < ciphertexts.size(); i+=3)
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


    size_t total_bytes_rec = 0;
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