#include <stdio.h>
#include <stdlib.h>
#include "../Bmp_Parser.h"
#include "../type/image.h"
#include "cut.h"

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

//prints all values from an array
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
    char path[] = "../dataset/testTEXT.bmp";
    Image image;
    image.h = getHeight(path);
    image.w = getWidth(path);
    //unsigned char final_array[ image.h * image.w ];
    unsigned char *final_array = NULL;
    final_array = malloc((image.h * image.w) * sizeof(unsigned char));
    if (final_array == NULL)
    {
        printf("Not enough memory avaible !\n");
        return 1;
    }
    image.data = final_array;
    if (parse_bmp(final_array, path)==1)
    {
        printf("File not found !\n");
        return 1;
    }

    /*
    tests pour marge cutter CutBorder

    Rect edited = CutBorder(image);
    DrawRect(edited, image);
    printf("%zu %zu\n", image.w, image.h);
    printf("%i\n", image.data[(5*image.w)-2]);
    printf("%i %i %i %i\n", edited.topLeft.x, edited.downRight.x, edited.topLeft.y, edited.downRight.y);
    array_to_bmp(image.data, image.w, image.h, path);
    */


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