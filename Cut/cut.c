#include <stdio.h>
#include "cut.h"

//rect de position autour du contenu de la page, decoupage de la marge
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

Image cutLine(Image image, Rect rect)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    int active = 1;
    Rect inrect;
    inrect.topLeft.x = rect.topLeft.x;
    inrect.downRight.x = rect.downRight.x;
    int endit = 0;
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
                    inrect.topLeft.y = ( y-1 == -1 )? 0 : y-1;
                    active = 1;
                }
                break;
            }
        }
        if (x == rect.downRight.x && active)
        {
            active = 0;
            DrawRect(inrect, &result);
            inrect.downRight.y = y;
        }
    }
    return result;
}

void DrawRect(Rect rect, Image *image)
{
    int i = 0;
    while (i < 1000000000){i+=1;}
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
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int pos = y * image->w;
        (*image).data[pos] = 9;
    }
}