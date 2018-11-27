#ifndef BMP_PARSER_H
#define BMP_PARSER_HW

#include "type/image.h"

int load_image(char path[], Image *image);
int array_to_bmp(unsigned char *arr, size_t w, size_t h, char path[], char *n);
int parse_bmp(unsigned char *final_array, char path[]);
size_t getWidth(char path[]);
size_t getHeight(char path[]);

#endif
