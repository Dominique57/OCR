#include <stdio.h>
#include "../Bmp_Parser.h"
#include "../type/image.h"
#include "cut.h"

void print_Array(unsigned char *array, size_t w, size_t h) //prints all values from an array
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

void print_image(Image im) //prints all values from an array
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
    char path[] = "../dataset/testTEXT.bmp";
    Image image;
    image.h = getHeight(path);
    image.w = getWidth(path);
    unsigned char final_array[ image.h * image.w ];
    printf("%zu\n",image.h * image.w);
    image.data = final_array;
    if (parse_bmp(final_array, path)==1)
    {
        printf("File not found !\n");
        return 1;
    }
    // print_image(image);
    // printf("\n\n\n");
    Image edited;
    Rect rect;
    Cord left;
    left.x = 0;
    left.y = 0;
    rect.topLeft = left;
    Cord right;
    right.x = image.w;
    right.y = image.h;
    rect.downRight = right;
    edited = cutLine(image, rect);
    // print_image(edited);
    array_to_bmp(edited.data, edited.w, edited.h, path);
    return 0;
}