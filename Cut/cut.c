#include <stdio.h>
#include "../type/image.h"

//rect de position autour du contenu de la page, decoupage de la narge
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

void cutLine(Image image, Rect rect)
{
    int sumbpx = 0;
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        for (int  = rect.topLeft.x; x < rect.downRight.x; ++x)
        {
            int pos = y * image.w + x;
            if  (image.data[x] == 1)
            {
                sumbpx++;
            }
            else
            {

            }
        }

        sumbpx = 0;
    }
}

void cutLine(Image image, Rect rect)
{
    int nbWhiteLine = 0;
    int nbLine = 0;
    for (int y = rect.topLeft.y; y < rect.downRight.y; ++y)
    {
        int keep = 1;
        for (int  = rect.topLeft.x; x < rect.downRight.x && keep; ++x)
        {
            int pos = y * image.w + x;
            if  (image.data[x] == 1)
            {
                keep = 0
            }
        }

        sumbpx = 0;
    }
}