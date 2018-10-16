#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
Compile with gcc -Wall -Wextra -std=c99 -lSDL2 -lSDL2_image
*/

SDL_Surface *image=NULL; //surface initialization

struct Pixel { //pixel structure
    int r;
    int g;
    int b;
} Pixel;

struct Pixel get_color(SDL_Surface *image, int x, int y) //returns a Pixel at x,y coordinates with it's color attributes from a surface
{
	uint8_t *pixel=(uint8_t *)image->pixels + y * image->pitch + x*image->format->BytesPerPixel; //finds the pixel
	struct Pixel pix;
	
	pix.r=pixel[0];
	pix.g=pixel[1];
	pix.b=pixel[2];
	
	return pix;
}


int get_color_array(SDL_Surface *image, size_t w, size_t h, unsigned char *final_array) //puts all pixels from a surface in supplied array
{
	struct Pixel pix;
	float average=0;
	int k=0;
	for (size_t i=0; i<h; i++)
	{
		for (size_t j=0; j<w; j++)
		{
			pix=get_color(image, j, i);
			average=((float)pix.r+(float)pix.g+(float)pix.b)/(float)3; //(r+g+b)/3
			
			final_array[k]=average<128;
			
			k++;
		}
	}
	return 0;
}

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


	
	

	get_color_array(image, w, h, final_array); //fills the pixel array with all the pixels of the image
	

	SDL_FreeSurface(image); //cleanup
	return 0;


}

int array_to_bmp(unsigned char *final_array, size_t width, size_t height, char path[])
{
	image=IMG_Load(path);
	size_t w=width;
	size_t h=height;
	size_t k=0;
	uint8_t *pixel;
	int toPut;
	for(size_t i=0; i<w; i++)
	{
		for (size_t j=0; j<h; j++)
		{
			pixel=(uint8_t *)image->pixels + i * image->pitch + j*image->format->BytesPerPixel;
			if (final_array[k]==0)
			{
				toPut=255;
			}
			else
			{
				toPut=0;
			}
			pixel[0]=toPut;
			pixel[1]=toPut;
			pixel[2]=toPut;
			k++;
		}
	}
	SDL_SaveBMP(image, "image001.bmp"); //save surface to bmp file
	return 0;
}
