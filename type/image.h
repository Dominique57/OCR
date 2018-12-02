#ifndef image_H
#define image_H

#include <stdlib.h>
#include <stdio.h>

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

typedef struct ListChar ListChar;
struct ListChar
{
	ListChar *next;
	ListChar *prev;
	Rect pos;
	int type;
};

typedef struct ListHead ListHead;
struct ListHead
{
	void *head;
	void *tail;
};

typedef struct Image Image;
struct Image
{
    size_t w;
    size_t h;
    unsigned char *data;
    Image* copy;
};

typedef struct ListImage ListImage;
struct ListImage
{
	ListImage *prev;
	ListImage *next;
	Image *image;
	char carac;
};


void AddListChar(ListHead *list, ListChar *elt);
int IsEmpty(ListHead *list);
ListChar *InitListChar();
void FreeListChar(ListChar *list);
void FreeList(ListHead *list);
ListHead *InitListHead();
void PrintListType(ListHead *list);
void CopyRect(Rect src, Rect *dst);
ListImage *InitListImage();
void AddListImage(ListHead *list, ListImage *image);
void FreeListImage(ListImage *listImage);
void FreeListHeadImage(ListHead *list);
#endif