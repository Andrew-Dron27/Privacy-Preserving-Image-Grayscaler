#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "image.h"
#include <iostream>

Image::Image(){

}

Image::Image(const char* file_name) {
	
    if(read_image(file_name)){
		std::cout << "Succesfully read image: " << file_name << "\n";
		size = width * height * channels;
    }
    else{
		std::cout << "Failed to read image: " << file_name << "\n";
    }
}

Image::Image(uint8_t* _data, int size){
	std::copy(_data, _data + size, data);
}

Image::Image(int w, int h, int channels) : width(w), height(h), channels(channels) {
	size = w*h*channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.width, img.height, img.channels) {
	memcpy(data, img.data, size);
}

Image::~Image() {
	stbi_image_free(data);
}

bool Image::read_image(const char* image_path) {
    data = stbi_load(image_path, &width, &height, &channels, 0);
    return data != NULL;
}

bool Image::write_image(const char* image_path) {;
    int success = stbi_write_jpg(image_path, width, height, channels, data, 100);
	if(success != 1){
		std::cout << "Failed writing image to filepath: " << image_path << "\n";
		return false;
	}
	std::cout << "Successfully wrote image to filepath: " << image_path << "\n";
	return true;
}

Image& Image::grayscale_lum(){
	float red_lum = 0.21;
	float green_lum = 0.72;
	float blue_lum = 0.07;
	for(int i = 0; i < size; i+=channels){
		int gray = (floor(data[i] * red_lum) + floor(data[i+1] * green_lum) + floor(data[i+2] * blue_lum));
		memset(data+i, gray, 3);
	}
	return *this;
}