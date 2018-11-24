#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Bmp_Parser.h"
#include "../type/image.h"
#include "cut.h"
//gcc -Wall -Wextra -std=c99 -lSDL2 -lSDL2_image -o main *.c

//prints all values from an array
void print_Array(unsigned char *array, size_t w, size_t h)
{
    size_t k;
    k=0;
    for(size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<w; j++)
        {
            printf("%d",array[k]);
            k++;
        }
        printf("\n");
    }
}

//prints terminal max width values from an array
void print_image(Image im)
{
    unsigned char *array = im.data;
    size_t h = im.h;
    size_t w = im.w;
    size_t k;
    k=0;
    for(size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<237; j++)
        {
            printf("%d",array[k]);
            k++;
        }
        for (size_t l = 237; l <w ; ++l)
        {
            k++;
        }
        printf("\n");
    }
}


int main()
{
    char path[] = "../dataset/tryme.bmp";
    Image image1;
    load_image(path, &image1);
    Image result = image1;
    result = Parse_Image(image1, 0);

    array_to_bmp(result.data, result.w, result.h, path, "test0.bmp");

    return 0;
}
