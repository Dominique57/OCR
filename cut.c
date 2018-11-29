#include <stdio.h>
#include <stdlib.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "type/cut.h"
#include "resize.h"
#include "NN.h"



/*
 * determines the average space between caracters of a line
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the line
 */
int GetLineThresold(Image image, Rect line)
{
    // go to first col with black pixels
    int keep = 1;
    int x = line.topLeft.x;
    for (; x <= line.downRight.x && keep; ++x)
    {
        for (int y = line.topLeft.y; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
    }

    int active = 0;
    unsigned long colCount = 0;
    unsigned long bufferCount = 0;
    unsigned long spaceCount = 0;

    /*
    for debugging purposes
    Rect rect;
    rect.topLeft.y = line.topLeft.y;
    rect.downRight.y = line.downRight.y;
    */

    // first line with black piexls,
    // count every space and length till no black pixels no more
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                if (active == 1)
                {
                    spaceCount++;
                    colCount += bufferCount;
                    bufferCount = 0;
                    /*
                     for debugging purposes
                    rect.downRight.x = x - 1;
                    DrawRect(rect, image, 3, 3);
                    */
                }
                active = 0;
                break;
            }

        }
        if  (y > line.downRight.y)
        {
            if (active == 0)
            {
                active = 1;
                // for debugging purposes
                // rect.topLeft.x = x;
            }
            bufferCount++;
        }
    }
    int value = (spaceCount != 0) ? ( colCount / spaceCount )  : 0;
    return value * 1.6;
}

/*
 * Sends a rect of the image without white borders (no need to save them)
 * param :
 *      image: image where informations will be read
 */
Rect CutBorder(Image image)
{
    Cord topleft;
    topleft.x = 0;
    topleft.y = 0;

    Cord downRight;
    downRight.x = image.w;
    downRight.y = image.h;

    Rect rect;
    rect.topLeft = topleft;
    rect.downRight = downRight;

    // left border
    unsigned keep = 1;
    for (size_t x = 0; x < image.w && keep; ++x)
    {
        for (size_t y = 0; y < image.h && keep; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                rect.topLeft.x = x;
                keep = 0;
            }
        }
    }
    // right border
    keep = 1;
    for (size_t x = image.w-1; x > 0 && keep; --x)
    {
        for (size_t y = 0; y < image.h && keep; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                rect.downRight.x = x;
                keep = 0;
            }
        }
    }
    // upper border
    keep = 1;
    for (size_t y = 0; y < image.h && keep; ++y)
    {
        for (size_t x = 0; x < image.w && keep; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                rect.topLeft.y = y;
                keep = 0;
            }
        }
    }
    // down border
    keep = 1;
    for (size_t y = image.h-1; y > 0 && keep; --y)
    {
        for (size_t x = 0; x < image.w && keep; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                rect.downRight.y = y;
                keep = 0;
            }
        }
    }
    // showing the cutted border
    // DrawRect(rect, image, 2, 2);
    return rect;
}

/*
 * Sends back a copy of a given Image struct
 * param :
 *      image: image where informations will be read
 *      dest : pointer to image where data will be copied
 */
void CopyImage(Image image, Image *dest)
{
	if(dest == NULL)
		return;
	size_t arrSize = (image.h * image.w);
	dest->w = image.w;
	dest->h = image.h;

	if(dest->data)
	    free(dest->data);

	dest->data = malloc(arrSize * sizeof(unsigned char));
	if(dest->data == NULL)
    {
	    printf("No free memory availble ! Image copy impossible !\n");
	    return;
    }

    for (size_t k = 0; k < arrSize; ++k)
    {
        dest->data[k] = image.data[k];
    }
}

/*
 * Frees the image, that is to say the malloc of data
 * param :
 *      image : image to free
 */
void FreeImage(Image image)
{
    if(image.data)
        free(image.data);
    if(image.copy)
        FreeImage(*(image.copy));
}

/*
 * For a given image in a rect zone, detects lines
 * It also applies caracter detection and eventually space detection
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the block
 *      result: image where graphical result will be saved
 *      f: file in which OCR result will be written
 *      w1 and w2, neural network parameters
 */

void cutLine(Image image, Rect rect, FILE *f, float *w1, float *w2, char **text)
{
    /*
    Image result;
    result.w = image.w;
    result.h = image.h;
    result.data = image.data;
    */
    int active = 0;
    Rect inrect;
    inrect.topLeft.x = rect.topLeft.x;
    inrect.downRight.x = rect.downRight.x;
	int y = rect.topLeft.y;
    for (; y <= rect.downRight.y; ++y)
    {
        int x = rect.topLeft.x;
        for (; x <= rect.downRight.x; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                if (active == 0)
                {
                    inrect.topLeft.y = y;
                    active = 1;
                }
                break;
            }
        }
        if (x > rect.downRight.x && active == 1)
        {
            active = 0;
            inrect.downRight.y = y - 1; //downright est exclus, pas y-1
            DrawRect_hor(inrect, *(image.copy), 2);
            CutChar2(image, inrect, f, w1, w2, text);
            fputc('\n', f);
        }
    }
	if(active)
	{
		inrect.downRight.y = y - 1;
		DrawRect_hor(inrect, *(image.copy), 2);
		CutChar2(image, inrect, f, w1, w2, text);
		fputc('\n', f);
	}
}

/*
 * Applies caracter cut of the image in the line specified bu rect
 * Also writes in FILE f the position of detected caracters and spaces too
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the line
 *      result: image where graphical result will be saved
 *      f: file in which OCR result will be written
 */
void CutChar(Image image, Rect line, FILE *f)
{
    int active = 0;
    Rect charPos;
    charPos.topLeft.y = line.topLeft.y;
    charPos.downRight.y = line.downRight.y;
    int x = line.topLeft.x;
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1 )
            {
                if  (active == 0)
                {
                    charPos.topLeft.x = x;
                    active = 1;
                }
                break;
            }
        }
        if  (y > line.downRight.y && active == 1)
        {
            active = 0;
            charPos.downRight.x = x - 1;
            fputc('C', f);
            DrawRect_ver(charPos, image, 3);
        }
    }
    if(active)
    {
        charPos.downRight.x = x - 1;
        fputc('C', f);
        DrawRect_ver(charPos, image, 3);
    }
}

/*
 * Applies caracter cut of the image in the line specified bu rect
 * AND calculates linethresold to estimate average space and detect spaces
 * Also writes in FILE f the position of detected caracters and spaces too
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the line
 *      result: image where graphical result will be saved
 *      f: file in which OCR result will be written
 *      w1 and w2, neural network parameters
 */

void CutChar2(Image image, Rect line, FILE *f, float *w1, float *w2, char **text)
{
    int thresold = GetLineThresold(image, line);

    int xl = line.topLeft.x, xr = line.topLeft.x;

    int active = 0;
    Rect charPos;
    charPos.topLeft.y = line.topLeft.y;
    charPos.downRight.y = line.downRight.y;
    int x = line.topLeft.x;
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1 )
            {
                if  (active == 0)
                {
                    charPos.topLeft.x = x;
                    active = 1;
                    // compared computed space to thresold
                    xr = x;
                    if (xr - xl > thresold)
                    {
                        Rect rect;
                        rect.topLeft.x = xl;
                        rect.downRight.x = xr-1;
                        rect.topLeft.y = line.topLeft.y;
                        rect.downRight.y = line.downRight.y;
                        DrawRect_hor(rect, image, 4);
                        fputc(' ', f);
                    }

                }
                break;
            }
        }
        if  (y > line.downRight.y)
        {
            if (active == 1)
            {
                active = 0;
                charPos.downRight.x = x - 1;
                CharProcess(image, charPos, f, w1, w2, text);
                DrawRect(charPos, *(image.copy), 3, 3);
                xl = x;
            }
        }
    }
    if(active)
    {
        charPos.downRight.x = x - 1;
        CharProcess(image, charPos, f, w1, w2, text);
        DrawRect(charPos, *(image.copy), 3, 3);
    }
}

/*
 * Applies caracter cut of the image in the line specified bu rect
 * AND calculates linethresold to estimate average space and detect spaces
 * Also writes in FILE f the position of detected caracters and spaces too
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the char
 *      result: image where graphical result will be saved
 *      f: file in which OCR result will be written
 *      w1 and w2, neural network parameters
 */

void CharProcess(Image image, Rect rect, FILE *f, float *w1, float *w2, char **text)
{
    // check if multiple caracters in the same rect
	unsigned char resized[256];
	resize(image, rect, resized);
	unsigned char carac = 0;
	if(*text != NULL)
    {
	    do
	    {
            carac = *text[0];
            if(carac == '\0')
            {
                *text = NULL;
                break;
            }
            else
            {
                //printf("Associated car %c\n\n", carac);
                *text = *text + 1;
            }
	    }while (carac == ' ');

    }
    char output = Prediction(resized, w1, w2, carac);
	fputc(output, f);
}

/*
 * action to do when character position found
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      hor_val: horizontal value to apply
 *      ver_val: vertical value to apply
 * Take note : corners will be defined by hor_val
 */
void DrawRect(Rect rect, Image image, int hor_val, int ver_val)
{
    DrawRect_ver(rect, image, ver_val);
    DrawRect_hor(rect, image, hor_val);
}

/*
 * Draws the horizontal borders of the rect in given image
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      val: value to apply
 */
void DrawRect_hor(Rect rect, Image image, int val)
{
    int ypos = rect.topLeft.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = val;
    }
    ypos = rect.downRight.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = val;
    }
}

/*
 * Draws the vertical borders of the rect in given image
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      val: value to apply
 */
void DrawRect_ver(Rect rect, Image image, int val)
{
    int xpos = rect.topLeft.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image.w;
        image.data[pos] = val;
    }
    xpos = rect.downRight.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image.w;
        image.data[pos] = val;
    }
}

/*
 * Calling function applying the image segmentation to a given image struct
 * param :
 *      image: image where informations will be read
 *      newImage: bool to know if image shoould be modified or newly created
 */
Image Parse_Image(Image image, char **text, float *w1, float *w2)
{
    Image result = image;
    Rect border;
    /* // only use if cutborder breaks code
    Cord left;
    left.x = 0;
    left.y = 0;
    Cord right;
    right.x = image.w;
    right.y = image.h;
    border.topLeft = left;
    border.downRight = right;
    */

    //cut border of image
    border = CutBorder(image);

    //create file for output
    FILE *file = fopen("output.txt", "w+");

    if (file == NULL)
    {
        printf("Unexpected error when opening file !");
    }

    cutLine(image, border, file, w1, w2, text);

    fclose(file);

    return result;
}

Image cut2(char *path, char *text)
{
    Image image1;
    load_image(path, &image1);
    Image copy;
    CopyImage(image1, &copy);
    image1.copy = &copy;

    //load NN
    //size_t nbInput = 256, nbHidden = 256, nbOutput = 72;
    float w1[nbInput * nbHidden + nbHidden];
    float w2[nbHidden * nbOutput + nbOutput];
	
	//if file does not exist, uncomment, run and comment following lines
	//Initialization(w1, w2, 0);
	//SaveNetwork(w1, w2);
    Initialization(w1, w2, 1);

    char **textPointer = &text;
    int isText = (text)? 1 : 0;

	image1 = Parse_Image(image1, textPointer, w1, w2);

    if(isText)
    {
        if(*textPointer == NULL || **textPointer != '\0')
        {
            printf("TEXTE CORRESPOND PAS A LA DETECTION DE L'IMAGE!\n\n");
            Initialization(w1, w2, 1);
        }
        else
		{
			SaveNetwork(w1, w2);
		    printf("Texte correspond !(ou pas de texte)\n\n");
		}
    }

    return *(image1.copy);
}

Image cut(char *path, char *text)
{
    Image image1;
    image1.data = NULL;
    image1.copy = NULL;
    load_image(path, &image1);
    Image copy;
    copy.data = NULL;
    copy.copy = NULL;
    CopyImage(image1, &copy);
    image1.copy = &copy;

    //load NN
    //size_t nbInput = 256, nbHidden = 256, nbOutput = 72;
    float w1[nbInput * nbHidden + nbHidden];
    float w2[nbHidden * nbOutput + nbOutput];
    Initialization(w1, w2, 0);
    //SaveNetwork(w1, w2);


    int isText = (text)? 1 : 0;;
    char **textPointer = &text;
    int i = 1;
    do
    {
        char **textPointer = &text;
        image1 = Parse_Image(image1, textPointer, w1, w2);
    } while(i++ <= 100);


    if(isText)
    {
        if(textPointer)
        {
            if(*textPointer == NULL || **textPointer != '\0')
                printf("TEXTE CORRESPOND PAS A LA DETECTION DE L'IMAGE!\n\n");
                // TODO : do not UPDATE weights
            else
            {
                SaveNetwork(w1, w2);
                printf("Texte correspond !(ou pas de texte)\n\n");
                // save weights
            }
        }
        else
            printf("Unknow error !\n");
    }


    return *(image1.copy);
}
