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
	ListChar *head;
	ListChar *tail;
};

typedef struct Image Image;
struct Image
{
    size_t w;
    size_t h;
    unsigned char *data;
    Image* copy;
};


void AddListChar(ListHead *list, ListChar *elt);
int IsEmpty(ListHead *list);
ListChar *InitListChar();
void FreeListChar(ListChar *list);
void FreeList(ListHead *list);
ListHead *InitListHead();
void PrintListType(ListHead *list);
void CopyRect(Rect src, Rect *dst);

#endif