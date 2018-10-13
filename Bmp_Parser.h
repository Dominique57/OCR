#ifndef BMP_PARSER_H
#define BMP_PARSER_HW

int parse_bmp(unsigned char *final_array, char path[]);
size_t getWidth(char path[]);
size_t getHeight(char path[]);

#endif
