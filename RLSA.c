#include <stdio.h>
#include "type/image.h"

Image RLSA_v(Image image, int thresh)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    unsigned char data[result.w * result.h];
    result.data = data;
    for (size_t i = 0; i < image.w; i++)
    {
        int count = 0;
        int flag = 1;
        for (size_t j = 0; j < image.h; j++)
        {
            int pos = i * image.h +  j;
            if (image.data[ pos ] == 0) // si blanc
            {
                flag = 0;
                count++;
            }
            else
            {
                if (flag == 0 && count <= thresh)
                {
                    // TODO : set pixel black following this rule
                    // output(Rect(i, j - count, 1, count)).setTo(Scalar::all(0));
                }
                flag = 1;
                count = 0;
            }
        }
    }
    return result;
}

Image RLSA_h(Image image, int thresh)
{
    Image result;
    result.w = image.w;
    result.h = image.h;
    unsigned char data[result.w * result.h];
    result.data = data;
    for (size_t j = 0; j < image.h; j++)
    {
        int count = 0;
        int flag = 1;
        for (size_t i = 0; i < image.w; i++)
        {
            int pos = j * image.w +  i;
            if (image.data[ pos ] == 0)
            {
                flag = 0;
                count++;
            }
            else
            {
                if (flag == 0 && count <= thresh)
                {
                    // output(Rect(i - count, j, count, 1)).setTo(Scalar::all(0));
                }
                flag = 1;
                count = 0;
            }
        }
    }
    return result;
}

Image RLSAassemble(Image image1, Image image2)
{
    Image result;
    result.w = image1.w;
    result.h = image1.h;
    unsigned char data[result.w * result.h];
    result.data = data;
    for (size_t i = 0; i < image1.w; ++i)
    {
        for (size_t j = 0; j < image1.h; ++j)
        {
            int pos = i * image1.h + j;
            if (image1.data[pos] == 1 && image2.data[pos] == 1)
            {
                result.data[pos] = 1;
            }
            else
            {
                result.data[pos] = 0;
            }
        }
    }
    return result;
}

Image RLSA_launch(Image image, int offsetv, int offseth)
{
    return RLSAassemble(RLSA_h(image, offseth), RLSA_v(image, offsetv));
}