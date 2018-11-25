#include <stdio.h>
#include "type/image.h"



void print_array(unsigned char *array, size_t w, size_t h, unsigned char name[])
        //prints all values from an array
{
		printf("Array : %s\n", name);
        for(size_t i=0; i<h; i++)
        {
                        for (size_t j=0; j<w; j++)
                        {
                                printf("%i|",array[i*w+j]);
                        }
                        printf("\n");
        }
		printf("\n");
}


int resize_mat_nearest_neighbours(unsigned char resizedMat[256], unsigned char baseLetter[], int xlen, int ylen)
{
	int maxlen=xlen;
	if (xlen<=0 || ylen<=0)
	{
		printf("resize_mat: invalid lengths: xlen=%i, ylen=%i\n", xlen, ylen);
		return 1;
	}
	if (maxlen<ylen)
	{
		maxlen=ylen;
	}

	const float rowsRatio=(float)ylen/16;
	const float colsRatio=(float)xlen/16;

	unsigned char posRows[16];
	unsigned char posCols[16];
	unsigned char colsInterpolated[ylen*16];
	for (size_t i=0; i<16; i++)
	{
		posRows[i]=(int)(i*rowsRatio);
		posCols[i]=(int)(i*colsRatio);
	}
	for (size_t i=0; i<(ylen*16); i++)
	{
		colsInterpolated[i]=0;
	}
	//print_array(posRows, 16, 1, "posRows");
	//print_array(posCols, 16, 1, "posCols");
	//print_array(baseLetter, xlen, ylen, "baseLetter");
	//printf("rowsRatio: %f; colsRatio:%f; 5/rowsRatio:%f; xlen:%i; ylen:%i\n", rowsRatio, colsRatio, 5/rowsRatio, xlen, ylen);

	unsigned char tmp_value=0;
	size_t tmp_i=0;
	for (size_t i=0; i<ylen; i++) //cols interpolation
	{
		for (size_t j=0; j<xlen; j++)
		{
			tmp_value=baseLetter[i*xlen+j];
			//printf("%i|",tmp_value);
			while (tmp_i<16 && (posCols[tmp_i]<=j))
			{
				if (posCols[tmp_i]==j)
				{
					colsInterpolated[i*16+tmp_i]=tmp_value;
				}
				tmp_i++;
			}
		}
		//printf("\n");
		tmp_i=0;
	}
	//printf("xlen:%i, ylen:%i\n", xlen, ylen);
	//print_array(colsInterpolated, 16, ylen, "colsInterpolated (after)");

	for (size_t i=0; i<16; i++) //rows interpolation
	{
		for (size_t j=0; j<ylen; j++)
		{
			tmp_value=colsInterpolated[j*16+i];
			//printf("%i|", tmp_value);
			while (tmp_i<16 && (posRows[tmp_i]<=j))
			{
				if (posRows[tmp_i]==j)
				{
					resizedMat[tmp_i*16+i]=tmp_value;
				}
				tmp_i++;
			}
		}
		tmp_i=0;
		//printf("\n");
	}
	
	//print_array(resizedMat, 16, 16, "resizedMat after rows interpolation");

	return 0;
}



void resize(Image image, Rect rect, unsigned char resizedMat[256])
{
	Cord topLeft=rect.topLeft;
	Cord downRight=rect.downRight;

	int xlen=downRight.x - topLeft.x+1;
	int ylen=downRight.y - topLeft.y+1;

	int maxlen=xlen;
	int maxlenSquared=xlen*xlen;
	if (ylen>maxlen)
	{
		maxlen=ylen;
		maxlenSquared=ylen*ylen;
	}

	unsigned char baseLetter[maxlenSquared];

	for (size_t i=0; i<256; i++)
	{
		resizedMat[i]=0;
	}


	size_t k=0;
	for (size_t i=topLeft.y; i<=downRight.y; i++)
	{
		for (size_t j=topLeft.x; j<=downRight.x; j++)
		{
			baseLetter[k]=image.data[i*image.w+j];
			k++;
		}
	}
	while (k<maxlenSquared)
	{
		baseLetter[k]=0;
		k++;
	}

	if(resize_mat_nearest_neighbours(resizedMat, baseLetter, xlen, ylen))
	{
		printf("resize: error resizing matrix\n");
	}

}
