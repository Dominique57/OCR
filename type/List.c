#include <stdlib.h>
#include <stdio.h>
#include "image.h"

void PrintListType(ListHead *list)
{
	if(IsEmpty(list))
		printf("List is empty !\n");
	else
	{
		ListChar *cur = list->head;
		while(cur)
		{
			printf("type : %i\n", cur->type);
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
	p = calloc(1, sizeof(ListChar));
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


