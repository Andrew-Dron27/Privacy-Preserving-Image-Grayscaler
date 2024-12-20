#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <fstream>
#include <vector>

#include "../image/image.h"
#include "../image/network.h"
#include "../image/image_encryptor.h"
#include <helib/helib.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void send_img_to_greyscale(int sock, char* file_read_path, char* file_write_path, int scalar_val, int scale_factor)
{
    std::cout << "Sending Image to server" << "\n";
    Image image = Image(file_read_path);

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

    helib::Context context = helib::ContextBuilder<helib::BGV>()
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

    //helib::Context context = ImageEncryptor::init_helib_context();

    context.printout();

    std::cout << "Creating secret Key" << "\n";
    // Create a secret key associated with the context.
	helib::SecKey secret_key(context);

	// Generate the secret key.
	secret_key.GenSecKey();

	// Generate bootstrapping data.
	secret_key.genRecryptData();

	// Public key management.
	// Set the secret key (upcast: SecKey is a subclass of PubKey).
	const helib::PubKey& public_key = secret_key;

	// Get the EncryptedArray of the context.
	const helib::EncryptedArray& ea = context.getEA();

	// Build the unpack slot encoding.
	std::vector<helib::zzX> unpackSlotEncoding;
	buildUnpackSlotEncoding(unpackSlotEncoding, ea);

	// Get the number of slot (phi(m)).
	//long nslots = ea.size();

    helib::Ptxt<helib::BGV> scalar(context);
    scalar[0] = scalar_val;
    helib::Ctxt enc_scalar(public_key);

    public_key.Encrypt(enc_scalar, scalar);

    //Image greyscale;
    int h = image.height;
    int w = image.width;
    if(send_public_key(sock, public_key))
    {
        
        helib::Ctxt ctxt_red(public_key);
        helib::Ctxt ctxt_green(public_key);
        helib::Ctxt ctxt_blue(public_key);
        ImageEncryptor::encrypt_image(image, public_key, context, ctxt_red, ctxt_green, ctxt_blue);

        if(send_encrypted_image(sock, ))
        {
            std::vector<enc_pixel_data> enc_data = recv_encrypted_pixels(sock, public_key);
            std::cout << "Decrypting image data" << "\n";
            Image greyscale = ImageEncryptor::decrypt_image_data(enc_data, context, secret_key, scale_factor, w, h);
            std::cout << "Writing image to file path: " << file_write_path << "\n";
            greyscale.write_image(file_write_path);
        }
    }

}

int main(int argc, char *argv[])
{
    int sockfd; 
    //char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 4) {
        fprintf(stderr,"Must supply hostname and filepath\n");
        exit(1);
    }

    char* file_read_path = argv[2];
    char* file_write_path = argv[3];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    // Image greyscale;

    // if(send_image(sockfd, image)){
    //     if(recv_image(sockfd,greyscale)){
    //         greyscale.write_image(file_write_path);
    //     }
    // }

    send_img_to_greyscale(sockfd, file_read_path, file_write_path, 33, 100);

    close(sockfd);

    return 0;
}

