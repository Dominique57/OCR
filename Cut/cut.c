#include <stdio.h>
#include "cut.h"
/*
 * Renvoie les coordonnees du contenue de la page hormis des marges
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

    //cherche bord gauche
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
    //cherche bord droit
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
    //cherche bord haut
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
    //cherche bord bas
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
    return rect;
}

/*
 * Prend une image en parametre et en renvoie une copie
 */
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

/*
 * Prend une image en parametre et la renvoie modifie avec decoupage
 * ligne ET char
 */
Image cutLine(Image image, Rect rect)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    result.data = image.data;
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
            DrawRect(inrect, result);
            CutChar(&image, inrect, &result);
        }
    }
    return result;
}

/*
 * Trace les lignes horizontales pour le decoupage par ligne
 */
void DrawRect(Rect rect, Image image)
{
    int ypos = rect.topLeft.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = 2;
    }
    ypos = rect.downRight.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = 2;
    }
}

/*
 * Applique le decoupage des caractere a l'image donnee en parametre
 */
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

/*
 * Applique les barrs verticales pour le decoupage des caracteres
 */
void DrawRect2(Rect rect, Image *image)
{
    int xpos = rect.topLeft.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image->w;
        image->data[pos] = 3;
    }
    xpos = rect.downRight.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image->w;
        image->data[pos] = 3;
    }

}