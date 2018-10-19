#ifndef BMP_PARSER_H
#define BMP_PARSER_HW

#include "type/image.h"

int load_image(char path[], Image *image);
int array_to_bmp(unsigned char *final_array, size_t width, size_t height, char path[]);
int parse_bmp(unsigned char *final_array, char path[]);
size_t getWidth(char path[]);
size_t getHeight(char path[]);
Image Parse_Image(Image image, int newImage);

#endif
