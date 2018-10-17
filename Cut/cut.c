#include <stdio.h>
#include "cut.h"

//rect de position autour du contenu de la page, découpage de la marge
/*
Rect CutBorder(Image image)
{
    Rect rect;
    Cord topleft;
    Cord downRight;
    topleft.x = 0;
    topleft.y = 0;
    downRight.x = image.w;
    downRight.y = image.h;
    rect.topLeft = topleft;
    rect.downRight = downRight;
    //cherche bord gauche
    unsigned keep = 1;
    for (size_t x = 0; x < image.w && keep; ++x)
    {
        for (size_t y = 0; y < image.h && keep; ++y)
        {
            int pos = x*image.h + y;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
        rect.topLeft.x++;
    }
    //cherche bord droit
    keep = 1;
    for (size_t x = image.w-1; x >= 0 && keep; --x)
    {
        for (size_t y = 0; y < image.h && keep; ++y)
        {
            int pos = x * image.h + y;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
        rect.downRight.x--;
    }
    //cherche bord haut
    keep = 1;
    for (size_t y = 0; y < image.h && keep; ++y)
    {
        for (size_t x = 0; x < image.w && keep; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
        rect.topLeft.y++;
    }
    //cherche bord bas
    keep = 1;
    for (size_t y = image.h-1; y >= 0 && keep; ++y)
    {
        for (size_t x = 0; x < image.w && keep; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
        rect.downRight.y--;
    }
    return rect;
}
*/


void print_rect(Image img, Rect rect)
{
    for(int y=rect.topLeft.y; y<rect.downRight.y; y++)
    {
        for (int x= rect.topLeft.y; x<230; x++)
        {
            int pos = y * img.w + x;
            printf("%d", img.data[pos]);
        }
        printf("\n");
    }
}

Image CopyImage(Image image)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    unsigned char data[ result.w * result.h ];
    result.data = data;

    int max = result.w * result.h;
    for (int k = 0; k < max; ++k)
    {
        data[k] = image.data[k];
    }
    return result;
}


Image cutLine(Image image, Rect rect)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    int active = 0;
    Rect inrect;

    inrect.topLeft.x = rect.topLeft.x;
    inrect.downRight.x = rect.downRight.x;
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int x = rect.topLeft.x;
        for (; x < rect.downRight.x; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                if (active == 0)
                {
                    inrect.topLeft.y = ( y == 0 )? 0 : y-1;
                    active = 1;
                }
                break;
            }
        }
        if (x == rect.downRight.x && active == 1)
        {
            active = 0;
            inrect.downRight.y = y;
            DrawRect(inrect, &result);
            CutChar(&image, inrect, &result);
        }
    }
    return result;
}

void DrawRect(Rect rect, Image *image)
{
    int ypos = rect.topLeft.y * (*image).w;
    for (int x = rect.topLeft.x; x < rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        (*image).data[pos] = 2;
    }
    ypos = rect.downRight.y * (*image).w;
    for (int x = rect.topLeft.x; x < rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        (*image).data[pos] = 2;
    }
    /*
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int pos = y * image->w;
        (*image).data[pos] = 2;
    }
    */
}

void CutChar(Image *image, Rect line, Image *result)
{
    int active = 0;
    Rect charPos;
    charPos.topLeft.y = line.topLeft.y;
    charPos.downRight.y = line.downRight.y;
    for (int x = line.topLeft.x; x < line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y < line.downRight.y; ++y)
        {
            int pos = y * image->w + x;
            if (image->data[pos] == 1 )
            {
                if  (active == 0)
                {
                    charPos.topLeft.x = ( x == 0 )? 0 : x-1;
                    active = 1;
                }
                break;
            }
        }
        if  (y == line.downRight.y && active == 1)
        {
            active = 0;
            charPos.downRight.x = x;
            DrawRect2(charPos, result);
        }
    }
}

void DrawRect2(Rect rect, Image *image)
{
    int xpos = rect.topLeft.x;
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int pos = xpos + y * image->w;
        image->data[pos] = 3;
    }
    xpos = rect.downRight.x;
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int pos = xpos + y * image->w;
        image->data[pos] = 3;
    }

}