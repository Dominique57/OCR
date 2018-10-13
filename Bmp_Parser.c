#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

SDL_Surface *image=NULL; //initialisation de la surface

struct Pixel {
    int r;
    int g;
    int b;
} Pixel;

struct Pixel get_color(SDL_Surface *image, int x, int y, int pixel_size)
{
	uint8_t *pixel=(uint8_t *)image->pixels + y * image->pitch + x*pixel_size;
	struct Pixel pix;
	
	pix.r=pixel[0];
	pix.g=pixel[1];
	pix.b=pixel[2];
	
	return pix;
}


int get_color_array(SDL_Surface *image, int pixel_size, size_t w, size_t h, unsigned char *final_array)
{
	struct Pixel pix;
	float moyenne=0;
	int k=0;
	for (size_t i=0; i<h; i++)
	{
		for (size_t j=0; j<w; j++)
		{
			pix=get_color(image, j, i, pixel_size);
			moyenne=((float)pix.r+(float)pix.g+(float)pix.b)/(float)3;
			
			final_array[k]=moyenne>128;
			
			//final_array[3*k]=pix.r;
			//final_array[3*k+1]=pix.g;
			//final_array[3*k+2]=pix.b;
			k++;
		}
	}
	return 0;
}


int parse_bmp(char path[])
{
	//charge l'image
	image=IMG_Load(path);
	

	if (image==NULL)
	{
		return 1; //fichier non trouve
	}
	size_t w=image->w;
	size_t h=image->h;
	printf("w:%ld, h:%ld\n", w, h); //affiche la largeur et hauteur de l'image

	int pixel_size=image->format->BytesPerPixel;
	printf ("surface_w:%d, pixel_size:%d\n",image->pitch,pixel_size);
	
	unsigned char final_array[w*h];
	get_color_array(image, pixel_size, w, h, final_array);
	
	
	printf("Pixel0: %d \n",final_array[0]);
	return 0;


}

int main()
{
	printf ("%d\n",parse_bmp("TEST2.bmp"));
	return 0;
}
