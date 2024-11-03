#ifndef IMAGE_H   
#define IMAGE_H
#include <cstddef>
#include <cstdint>

struct Image {
	uint8_t* data = NULL;
	size_t size = 0;
	std::vector<long> red_channel;
	std::vector<long> green_channel;
	std::vector<long> blue_channel;
	int width;
	int height;
	int channels;

	Image();
	Image(const char* file_name);
	Image(int w, int h, int channels);
	Image(uint8_t* image_data, size_t size, int w, int h);
	Image(const Image& img);
	~Image();

	bool read_image(const char* file_name);
	bool write_image(const char* file_name);

	void Image::get_channels(std::vector<long>& red, std::vector<long>& green, std::vector<long>& blue) const;
	
	Image& grayscale_avg();
	Image& grayscale_lum();
};

#endif