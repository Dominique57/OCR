#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "type/image.h"

/*
Compile with gcc -Wall -Wextra -std=c99 -lSDL2 -lSDL2_image
*/

SDL_Surface *image=NULL; //surface initialization

struct Pixel { //pixel structure
    int r;
    int g;
    int b;
} Pixel;

//returns a Pixel at x,y coordinates with it's color attributes from a surface
struct Pixel get_color(SDL_Surface *image, int x, int y)
{
    //finds the pixel
    uint8_t *pixel=(uint8_t *)image->pixels
                   + y * image->pitch
                   + x*image->format->BytesPerPixel;
    struct Pixel pix;

    pix.r=pixel[0];
    pix.g=pixel[1];
    pix.b=pixel[2];

    return pix;
}

//puts all pixels from a surface in supplied array
int get_color_array(SDL_Surface *image, size_t w, size_t h, unsigned char *arr)
{
    struct Pixel pix;
    float average=0;
    int k=0;
    for (size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<w; j++)
        {
            pix=get_color(image, j, i);
            //(r+g+b)/3
            average=((float)pix.r+(float)pix.g+(float)pix.b)/(float)3;

            arr[k]=average<140;

            k++;
        }
    }
    return 0;
}

/*
int get_color_array_otsu
 (SDL_Surface *image, size_t w, size_t h, unsigned char *final_array)
{
    struct Pixel pix;
    size_t k=0;
    unsigned long sum = 0;
    for (size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<w; j++)
        {
            pix=get_color(image, j, i);
            final_array[k] = ( pix.r + pix.g + pix.b ) / 3; //(r+g+b)/3
            sum += final_array[k];
            k++;
        }
    }
    unsigned long thresold = sum / k;
    k=0;
    for (size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<w; j++)
        {
            pix=get_color(image, j, i);
            //(r+g+b)/3
            float average=((float)pix.r+(float)pix.g+(float)pix.b)/(float)3;

            final_array[k]=average<thresold;

            k++;
        }
    }
    return 0;
}
*/

size_t getWidth(char path[])
{
    image=IMG_Load(path); //loads the image


    if (image==NULL)
    {
        return -1; //file not found
    }
    return image->w;
}

size_t getHeight(char path[])
{
    image=IMG_Load(path); //loads the image


    if (image==NULL)
    {
        return -1; //file not found
    }
    return image->h;
}

int parse_bmp(unsigned char *final_array, char path[])
{
    image=IMG_Load(path); //loads the image


    if (image==NULL)
    {
        return 1; //file not found
    }
    size_t w=image->w; //width of the image
    size_t h=image->h; //height of the image

    //fills the pixel array with all the pixels of the image
    get_color_array(image, w, h, final_array);

    SDL_FreeSurface(image); //cleanup
    return 0;
}
int load_image(char path[], Image *image)
{
    (*image).h = getHeight(path);
    (*image).w = getWidth(path);
    unsigned char *final_array = NULL;
    final_array = malloc(((*image).h * (*image).w) * sizeof(unsigned char));
    if (final_array == NULL)
    {
        printf("Not enough memory avaible !\n");
        return 1;
    }
    (*image).data = final_array;
    if (parse_bmp(final_array, path)==1)
    {
        printf("File not found !\n");
        return 1;
    }
    return 0;
}

int array_to_bmp(unsigned char *arr, size_t w, size_t h, char path[], char *n)
{
    image=IMG_Load(path);
    size_t k=0;
    uint8_t *pixel;
    int toPut;
    for(size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<w; j++)
        {
            pixel=(uint8_t *)image->pixels
                  + i * image->pitch
                  + j*image->format->BytesPerPixel;
            toPut = (arr[k] == 1)? 0 : 255;
            pixel[0]=toPut;
            pixel[1]=toPut;
            pixel[2]=toPut;
            if ( arr[k] == 2)
            {
                pixel[0]=255;
                pixel[1]=0;
                pixel[2]=0;
            }
            else if ( arr[k] == 3)
            {
                pixel[0]=0;
                pixel[1]=0;
                pixel[2]=255;
            }
            else if ( arr[k] == 4)
            {
                pixel[0]=0;
                pixel[1]=255;
                pixel[2]=0;
            }
            k++;
        }
    }
    if (n != NULL)
    {
        SDL_SaveBMP(image, n); //save surface to bmp file
    }
    else
    {
        SDL_SaveBMP(image, "image001.bmp"); //save surface to bmp file
    }
    return 0;
}