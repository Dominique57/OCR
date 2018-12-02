#include "type/image.h"

void CopyRect(Rect src, Rect *dst)
{
	dst->downRight.x = src.downRight.x;
	dst->downRight.y = src.downRight.y;
	dst->topLeft.x = src.topLeft.x;
	dst->topLeft.y = src.topLeft.y;
}

void PrintListType(ListHead *list)
{
	if(IsEmpty(list))
		printf("List is empty !\n");
	else
	{
		ListChar *cur = list->head;
		while(cur)
		{
			char car = 'c';
			if(cur->type == 1)
				car = ' ';
			else if (cur->type == 2)
				car = '\n';
			printf("%c", car);
			cur = cur->next;
		}
	}
}

ListHead *InitListHead()
{
	ListHead *p = NULL;
	p = malloc(sizeof(ListHead));
	if(!p)
		return NULL;
	p->head = NULL;
	p->tail = NULL;
}

void FreeList(ListHead *list)
{
	ListChar *cur = list->head;
	while(cur)
	{
		ListChar *next = cur->next;
		FreeListChar(cur);
		cur = next;
	}
	free(list);
}

void FreeListChar(ListChar *list)
{
	free(list);
}

ListChar *InitListChar()
{
	ListChar *p;
	p = malloc(sizeof(ListChar));
	if(!p)
		return NULL;
	p->next = NULL;
	p->prev = NULL;
	p->pos.topLeft.x = 0;
	p->pos.downRight.y = 0;
	p->pos.topLeft.x = 0;
	p->pos.downRight.y = 0;
	p->type = 0;
	return p;
}

int IsEmpty(ListHead *list)
{
	return list->head == NULL;
}

void AddListChar(ListHead *list, ListChar *elt)
{
	if(IsEmpty(list))
	{
		list->head = elt;
		list->tail = elt;
	}
	else
	{
		ListChar *last = list->tail;
		last->next = elt;
		elt->prev = last;
		list->tail = elt;
	}
}

ListImage *InitListImage()
{
	ListImage *p = NULL;
	p = malloc(sizeof(ListImage));
	if(!p)
		return NULL;
	p->prev = NULL;
	p->next = NULL;
	p->image = NULL;
	p->carac = 0;
}

void AddListImage(ListHead *list, ListImage *image)
{
	if(IsEmpty(list))
	{
		list->head = image;
		list->tail = image;
	}
	else
	{
		ListImage *last = list->tail;
		last->next = image;
		image->prev = last->next;
		list->tail = image;
	}
}

void FreeListImage(ListImage *listImage)
{
	free(listImage->image);
	free(listImage);
}

void FreeListHeadImage(ListHead *list)
{
	ListImage *cur = list->head;
	while(cur)
	{
		ListImage    *next = cur->next;
		FreeListImage(cur);
		cur = next;
	}
	free(list);
}