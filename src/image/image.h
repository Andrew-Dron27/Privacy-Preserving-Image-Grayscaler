#ifndef IMAGE_H   
#define IMAGE_H
#include <cstddef>
#include <cstdint>

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	int width;
	int height;
	int channels;

	Image();
	Image(const char* file_name);
	Image(int w, int h, int channels);
	Image(uint8_t* image_data, int size);
	Image(const Image& img);
	~Image();

	bool read_image(const char* file_name);
	bool write_image(const char* file_name);

	Image& grayscale_avg();
	Image& grayscale_lum();
};

#endif