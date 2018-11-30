#include "image.h"

int main()
{
    ListHead *list = InitListHead();
    ListChar *elt1 = InitListChar();
    elt1->type = 1;
    ListChar *elt2 = InitListChar();
    elt2->type = 2;
    ListChar *elt3 = InitListChar();
    elt3->type = 3;
    ListChar *elt4 = InitListChar();
    elt4->type = 4;
    ListChar *elt5 = InitListChar();
    elt5->type = 5;
    AddListChar(list, elt1);
    AddListChar(list, elt2);
    AddListChar(list, elt3);
    AddListChar(list, elt4);
    AddListChar(list, elt5);
    PrintListType(list);
    FreeList(list);
}