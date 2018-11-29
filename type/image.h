#ifndef image_H
#define image_H

typedef struct Image Image;
struct Image
{
    size_t w;
    size_t h;
    unsigned char *data;
    Image* copy;
};

typedef struct Cord Cord;
struct Cord
{
    int x;
    int y;
};

typedef struct Rect Rect;
struct Rect
{
    Cord topLeft;
    Cord downRight;
};

#endif
