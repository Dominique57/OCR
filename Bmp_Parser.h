#ifndef BMP_PARSER_H
#define BMP_PARSER_HW

#include "type/image.h"

int loadImage(Image *output, char path[]);
int array_to_bmp(unsigned char *final_array, size_t width, size_t height, char path[]);
int parse_bmp(unsigned char *final_array, char path[]);
size_t getWidth(char path[]);
size_t getHeight(char path[]);

#endif
