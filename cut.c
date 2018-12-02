#include <stdio.h>
#include <stdlib.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "type/cut.h"
#include "resize.h"
#include "NN.h"
#include <time.h>
#include <sys/time.h>


/*
 * determines the average space between caracters of a line
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the line
 */
int GetLineThresold(Image image, Rect line)
{
    // go to first col with black pixels
    int keep = 1;
    int x = line.topLeft.x;
    for (; x <= line.downRight.x && keep; ++x)
    {
        for (int y = line.topLeft.y; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                keep = 0;
            }
        }
    }

    int active = 0;
    unsigned long colCount = 0;
    unsigned long bufferCount = 0;
    unsigned long spaceCount = 0;

    // first line with black piexls,
    // count every space and length till no black pixels no more
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                if (active == 1)
                {
                    spaceCount++;
                    colCount += bufferCount;
                    bufferCount = 0;
                }
                active = 0;
                break;
            }

        }
        if  (y > line.downRight.y)
        {
            if (active == 0)
            {
                active = 1;
            }
            bufferCount++;
        }
    }
    int value = (spaceCount != 0) ? ( colCount / spaceCount )  : 0;
    return value * 1.6;
}

/*
 * Sends a rect of the image without white borders (no need to save them)
 * param :
 *      image: image where informations will be read
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

    // left border
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
    // right border
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
    // upper border
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
    // down border
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
    // showing the cutted border
    // DrawRect(rect, image, 2, 2);
    return rect;
}

/*
 * Sends back a copy of a given Image struct
 * param :
 *      image: image where informations will be read
 *      dest : pointer to image where data will be copied
 */
void CopyImage(Image image, Image *dest)
{
    if(dest == NULL)
        return;
    size_t arrSize = (image.h * image.w);
    dest->w = image.w;
    dest->h = image.h;

    if(dest->data)
        free(dest->data);

    dest->data = malloc(image.h * image.w * sizeof(unsigned char));
    if(dest->data == NULL)
    {
        printf("No free memory availble ! Image copy impossible !\n");
        return;
    }

    for (size_t k = 0; k < arrSize; ++k)
    {
        dest->data[k] = image.data[k];
    }
}

/*
 * Frees the image, that is to say the malloc of data
 * param :
 *      image : image to free
 */
void FreeImage(Image *image)
{
    if(image->data)
    {
        free(image->data);
        image->data = NULL;
    }
    if(image->copy)
    {
        FreeImage(image->copy);
        image->copy = NULL;
    }
}

Image *InitImage()
{
    Image *p = malloc(sizeof(Image));
    if(!p)
        return NULL;
    p->w = 0;
    p->h = 0;
    p->data = NULL;
    p->copy= NULL;
    return p;
}

void LoadImageData(ListHead *list)
{
    char carac = 'a';
    char path[] = "letters/normal/a.bmp";
    while(carac <= 'z')
    {
        path[15] = carac;
        LoadImageElt(list, path, carac);
        carac++;
    }
    carac = 'a';
    char path2[] = "letters/normal/caps-a.bmp";
    while(carac <= 'z')
    {
        path2[20] = carac;
        LoadImageElt(list, path2, 'A' + carac - 'a');
        carac++;
    }
    carac = '0';
    char path3[] = "letters/normal/0.bmp";
    while(carac <= '9')
    {
        path3[15] = carac;
        LoadImageElt(list, path3, carac);
        carac++;
    }
    /*
    ListImage *cur = list->head;
    while(cur)
    {
        print_Array(cur->image->data, 16, 16);
        cur = cur->next;
    }
     */
}

void LoadImageElt(ListHead *list, char *path, char carac)
{
    ListImage *listImage = InitListImage();
    Image *image = InitImage();
    listImage->image = image;
    listImage->carac = carac;
    load_image(path, image);
    if(image->data)
        AddListImage(list, listImage);
    else
        FreeListImage(listImage);
}

void cutLine2(Image image, Rect rect, ListHead *list)
{
    int active = 0;
    Rect inrect;
    inrect.topLeft.x = rect.topLeft.x;
    inrect.downRight.x = rect.downRight.x;
    int y = rect.topLeft.y;
    for (; y <= rect.downRight.y; ++y)
    {
        int x = rect.topLeft.x;
        for (; x <= rect.downRight.x; ++x)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1)
            {
                if (active == 0)
                {
                    inrect.topLeft.y = y;
                    active = 1;
                }
                break;
            }
        }
        if (x > rect.downRight.x && active == 1)
        {
            active = 0;
            inrect.downRight.y = y - 1; //downright est exclus, pas y-1
            DrawRect_hor(inrect, *(image.copy), 4);

            CutChar3(image, inrect, list);
            ListChar *listChar = InitListChar();
            listChar->type = 2;
            CopyRect(inrect, &(listChar->pos));
            AddListChar(list, listChar);
        }
    }
    if(active)
    {
        inrect.downRight.y = y - 1; //downright est exclus, pas y-1
        DrawRect_hor(inrect, *(image.copy), 4);

        CutChar3(image, inrect, list);
        ListChar *listChar = InitListChar();
        listChar->type = 2;
        CopyRect(inrect, &(listChar->pos));
        AddListChar(list, listChar);
    }
}

void CutChar3(Image image, Rect line, ListHead *list)
{
    int x = line.topLeft.x;
    int y = line.topLeft.y;
    NextCall(*(image.copy->copy), line, &x, &y);
    while(x != -1 /* && y != -1 */)
    {
        Rect charPos;
        charPos.topLeft.x = x;
        charPos.topLeft.y = y;
        charPos.downRight.x = x;
        charPos.downRight.y = y;
        CutCharRec(*(image.copy->copy), line, &charPos, y, x);
        ListChar *listChar = InitListChar();

        listChar->type = 0;
        CopyRect(charPos, &(listChar->pos));
        listChar->pos.topLeft.y = line.topLeft.y;
        int Hline = line.downRight.y-line.topLeft.y;
        int Hchar = listChar->pos.downRight.y - listChar->pos.topLeft.y;
        if(Hchar < 3*Hline/4)
            listChar->pos.downRight.y = listChar->pos.topLeft.y+3*Hline/4;
        AddListChar(list, listChar);
        CheckElement(list);

        NextCall(*(image.copy->copy), line, &x, &y);
    }
    ListChar *cur = list->tail;
    while (cur != NULL && (cur->type == 0 || cur->type == 1))
    {
        DrawRect(cur->pos, *(image.copy), 5, 5);
        cur = cur->prev;
    }
    AddSpace(list, image.copy);
}

void AddSpace(ListHead *list, Image *image)
{
    unsigned long spaceCount = 0;
    unsigned long elCount = 0;
    ListChar *cur = list->tail;
    if(cur != NULL && cur->type != 2)
    {
        //calculate all data
        while(cur->prev && cur->prev->type == 0)
        {
            ListChar *prev = cur->prev;
            int length = cur->pos.topLeft.x - prev->pos.downRight.x;
            if(cur->pos.topLeft.x > prev->pos.downRight.x&& length > 0)
            {
                spaceCount += length;
            }
            elCount++;
            cur = prev;
        }
        unsigned long th = (elCount == 0)? 0 : spaceCount / elCount;
        th *= 1.8;
        while (cur->next && cur->next->type != 2)
        {
            ListChar *next = cur->next;
            int isNotRiding = next->pos.topLeft.x > cur->pos.downRight.x;
            int length = next->pos.topLeft.x - cur->pos.downRight.x;
            if(isNotRiding && length > th)
            {
                ListChar *space = InitListChar();
                space->type = 1;
                cur->next = space;
                space->prev = cur;
                space->next = next;
                next->prev = space;

                space->pos.topLeft.x = cur->pos.downRight.x;
                space->pos.downRight.x = next->pos.topLeft.x;
                space->pos.topLeft.y = cur->pos.topLeft.y;
                space->pos.downRight.y = cur->pos.downRight.y;
                if(image)
                {
                    DrawRect(space->pos, *image, 3, 3);
                }
            }
            cur = next;
        }
    }
}

void CheckElement(ListHead *list)
{
    if(!IsEmpty(list))
    {
        ListChar *elt1 = list->tail;
        if (elt1 && elt1->type == 0)
        {
            ListChar *elt2 = elt1->prev;
            if (elt2 && elt2->type == 0)
            {
                int xlelt1 = elt1->pos.topLeft.x;
                int xrelt1 = elt1->pos.downRight.x;
                int xlelt2 = elt2->pos.topLeft.x;
                int xrelt2 = elt2->pos.downRight.x;
                int okElt1 = (xrelt1 - xlelt1 < 9) ? 1 : (xrelt1 - xlelt1) / 9;
                int okElt2 = (xrelt2 - xlelt2 < 9) ? 1 : (xrelt2 - xlelt2) / 9;
                if (xlelt1 + okElt1 >= xlelt2 && xrelt1 - okElt1 <= xrelt2)
                {
                    elt2->next = NULL;
                    list->tail = elt2;
                    if (elt1->pos.downRight.y > elt2->pos.downRight.y)
                        elt2->pos.downRight.y = elt1->pos.downRight.y;
                    if (elt1->pos.topLeft.x < elt2->pos.topLeft.x)
                        elt2->pos.topLeft.x = elt1->pos.topLeft.x;
                    if (elt1->pos.downRight.x > elt2->pos.downRight.x)
                        elt2->pos.downRight.x = elt1->pos.downRight.x;
                    FreeListChar(elt1);
                }
                else if (xlelt1 - okElt2 <= xlelt2 && xrelt1 - okElt2 >= xrelt2)
                {
                    if (elt2->pos.downRight.y > elt1->pos.downRight.y)
                        elt1->pos.downRight.y = elt2->pos.downRight.y;
                    if (elt1->pos.topLeft.x > elt2->pos.topLeft.x)
                        elt1->pos.topLeft.x = elt2->pos.topLeft.x;
                    if (elt1->pos.downRight.x < elt2->pos.downRight.x)
                        elt1->pos.downRight.x = elt2->pos.downRight.x;

                    elt2->type = elt1->type;
                    CopyRect(elt1->pos, &(elt2->pos));
                    elt2->next = NULL;
                    list->tail = elt2;

                    FreeListChar(elt1);
                    /*
                    somheow doesnt work
                    elt1->prev = elt2->prev;
                    if (elt2->prev)
                        elt2->prev = elt1;

                    /*
                    FreeListChar(elt2);
                     */
                }
            }
        }
    }
}

void NextCall(Image image, Rect line, int *xref, int *yref)
{
    int x = line.topLeft.x;
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if(image.data[pos] == 1)
            {
                *xref = x;
                *yref = y;
                return;
            }
        }
    }
    *xref = -1;
    *yref = -1;
}

void CutCharRec(Image image, Rect line, Rect *rect, int y, int x)
{
    int a = y <= line.downRight.y;
    int b = y >= line.topLeft.y;
    int c = x >= line.topLeft.x;
    int d = x <= line.downRight.x;
    if( a && b && c && d && image.data[y*image.w+x] == 1)
    {
        if (x > rect->downRight.x)
            rect->downRight.x = x;
        else if (x < rect->topLeft.x)
            rect->topLeft.x = x;
        if (y > rect->downRight.y)
            rect->downRight.y = y;
        else if (y < rect->topLeft.y)
            rect->topLeft.y = y;
        image.data[y*image.w+x] = 2;
        CutCharRec(image, line, rect, y-1, x-1);
        CutCharRec(image, line, rect, y-1, x);
        CutCharRec(image, line, rect, y-1, x+1);
        CutCharRec(image, line, rect, y, x-1);
        CutCharRec(image, line, rect, y, x+1);
        CutCharRec(image, line, rect, y+1, x-1);
        CutCharRec(image, line, rect, y+1, x);
        CutCharRec(image, line, rect, y+1, x+1);
    }
}

void CutChar2(Image image, Rect line, ListHead *list)
{
    int thresold = GetLineThresold(image, line);

    int xl = line.topLeft.x, xr = line.topLeft.x;

    int active = 0;
    Rect charPos;
    charPos.topLeft.y = line.topLeft.y;
    charPos.downRight.y = line.downRight.y;
    int x = line.topLeft.x;
    for (; x <= line.downRight.x; ++x)
    {
        int y = line.topLeft.y;
        for (; y <= line.downRight.y; ++y)
        {
            int pos = y * image.w + x;
            if (image.data[pos] == 1 )
            {
                if  (active == 0)
                {
                    charPos.topLeft.x = x;
                    active = 1;
                    // compared computed space to thresold
                    xr = x;
                    if (xr - xl > thresold)
                    {
                        Rect rect;
                        rect.topLeft.x = xl;
                        rect.downRight.x = xr-1;
                        rect.topLeft.y = line.topLeft.y;
                        rect.downRight.y = line.downRight.y;
                        DrawRect_hor(rect, *(image.copy), 3);

                        ListChar *listChar = InitListChar();
                        listChar->type = 1;
                        CopyRect(rect, &(listChar->pos));
                        AddListChar(list, listChar);
                    }
                }
                break;
            }
        }
        if  (y > line.downRight.y)
        {
            if (active == 1)
            {
                active = 0;
                charPos.downRight.x = x - 1;
                DrawRect(charPos, *(image.copy), 5, 5);
                xl = x;

                // CorrectRect(image, &charPos);

                ListChar *listChar = InitListChar();
                listChar->type = 0;
                CopyRect(charPos, &(listChar->pos));
                AddListChar(list, listChar);
            }
        }
    }
    if(active)
    {
        charPos.downRight.x = x - 1;
        DrawRect(charPos, *(image.copy), 5, 5);

        // CorrectRect(image, &charPos);

        ListChar *listChar = InitListChar();
        listChar->type = 0;
        CopyRect(charPos, &(listChar->pos));
        AddListChar(list, listChar);
    }
}

void CorrectRect(Image i, Rect *r)
{
    int active = 1;
    for (int y = r->downRight.y; y >= r->topLeft.y && active; --y)
    {
        for (int x = r->topLeft.x; x <= r->downRight.x; ++x)
        {
            int pos = y * i.w + x;
            if(i.data[pos] == 1)
            {
                r->downRight.y = y;
                active = 0;
                break;
            }
        }
    }
}

/*
 * Applies caracter cut of the image in the line specified bu rect
 * AND calculates linethresold to estimate average space and detect spaces
 * Also writes in FILE f the position of detected caracters and spaces too
 * param :
 *      image: image where informations will be read
 *      rect: rectangle around the char
 *      result: image where graphical result will be saved
 *      f: file in which OCR result will be written
 *      w1 and w2, neural network parameters
 */
void CharProcess(Image i, Rect r, FILE *f, float *w1, float *w2, char **t)
{
    /*
    int active = 1;
    for (int y = r.downRight.y; y >= r.topLeft.y && active; --y)
    {
        for (int x = r.topLeft.x; x <= r.downRight.x; ++x)
        {
            int pos = y * i.w + x;
            if(i.data[pos] == 1)
            {
                r.downRight.y = y;
                active = 0;
                break;
            }
        }
    }
    */
    // check if multiple caracters in the same rect
    unsigned char resized[256];
    resize(i, r, resized);
    unsigned char carac = 0;
    if(*t != NULL)
    {
        do
        {
            carac = *t[0];
            if(carac == '\0')
            {
                *t = NULL;
                break;
            }
            else
            {
                //printf("Associated car %c\n\n", carac);
                *t = *t + 1;
            }
        }while (carac == ' ' || carac == '\n');

    }
    char output = Prediction(resized, w1, w2, carac);
    if(f)
        fputc(output, f);
}

/*
 * action to do when character position found
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      hor_val: horizontal value to apply
 *      ver_val: vertical value to apply
 * Take note : corners will be defined by hor_val
 */
void DrawRect(Rect rect, Image image, int hor_val, int ver_val)
{
    DrawRect_ver(rect, image, ver_val);
    DrawRect_hor(rect, image, hor_val);
}

/*
 * Draws the horizontal borders of the rect in given image
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      val: value to apply
 */
void DrawRect_hor(Rect rect, Image image, int val)
{
    int ypos = rect.topLeft.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = val;
    }
    ypos = rect.downRight.y * image.w;
    for (int x = rect.topLeft.x; x <= rect.downRight.x; ++x)
    {
        int pos = ypos + x;
        image.data[pos] = val;
    }
}

/*
 * Draws the vertical borders of the rect in given image
 * param :
 *      rect: rectangle to draw
 *      image: image where grapgical result will be saved
 *      val: value to apply
 */
void DrawRect_ver(Rect rect, Image image, int val)
{
    int xpos = rect.topLeft.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image.w;
        image.data[pos] = val;
    }
    xpos = rect.downRight.x;
    for (int y = rect.topLeft.y; y <= rect.downRight.y; ++y)
    {
        int pos = xpos + y * image.w;
        image.data[pos] = val;
    }
}

Image Parse_Image2(Image image, ListHead *list)
{
    Rect border;
    border = CutBorder(image);
    cutLine2(image, border, list);
    return image;
}

void ReadList(Image i, FILE *f, ListHead *l, char **t, float *w1, float *w2)
{
    if(IsEmpty(l))
        return;
    ListChar *cur = l->head;
    while(cur)
    {
        if(cur->type == 0)
            // char
            CharProcess(i, cur->pos, f, w1, w2, t);
        else if(cur->type == 1 && f)
            // space
            fputc(' ', f);
        else if(cur->type == 2 && f)
            // end of line
            fputc('\n', f);
        cur = cur->next;
    }
}

Image cut_new(char *path, char *text, int learningIteration, int loadsaved)
{
    //Load image and a copy
    Image image1;
    image1.data = NULL;
    image1.copy = NULL;
    load_image(path, &image1);
    Image copy;
    copy.data = NULL;
    copy.copy = NULL;
    CopyImage(image1, &copy);
    image1.copy = &copy;
    Image copy2;
    copy2.data = NULL;
    copy2.copy = NULL;
    CopyImage(image1, &copy2);
    copy.copy = &copy2;

    //load NN
    float w1[nbInput * nbHidden + nbHidden];
    float w2[nbHidden * nbOutput + nbOutput];

    //if file does not exist, uncomment, run and comment following lines
    //Initialization(w1, w2, 0);
    //SaveNetwork(w1, w2);
    Initialization(w1, w2, loadsaved);

    //Create List and fill it
    ListHead *listHead = InitListHead();
    Parse_Image2(image1, listHead);


    //if we want to learn, do the iterations
    char *textPointer = text;
    char **textCur = &text;
    if(text)
    {
        ReadList(image1, NULL, listHead, textCur, w1, w2);
        int isTextCorrect = 0;
        //Check if given text, if any, is correctly sized
        if(*textCur)
        {
            while(**textCur == ' ' || **textCur == '\n')
            {
                *textCur+=1;
            }
        }
        if(*textCur == NULL || **textCur != '\0')
        {
            printf("TEXTE CORRESPOND PAS A LA DETECTION DE L'IMAGE!\n");
            if(*textCur == NULL)
                printf("Depassement du texte!\n");
            else
                printf("Texte non atteint la fin : '%c\n", **textCur);
        }
        else
        {
            isTextCorrect = 1;
            SaveNetwork(w1, w2);
            printf("Texte correspond !\n");
        }
        if(isTextCorrect)
        {
            while(--learningIteration > 0)
            {
                if(learningIteration%100==0)
                    printf("%i\n", learningIteration);
                if(learningIteration%1000==0)
                {
                    SaveNetwork(w1, w2);
                    printf("Network saved !\n");
                }
                *textCur = textPointer;
                ReadList(image1, NULL, listHead, textCur, w1, w2);
            }
            SaveNetwork(w1, w2);
        }

    }
    *textCur = textPointer;
    // note that after learning we do one execution to see the output

    //Create output file
    FILE *file = fopen("output.txt", "w+");
    if (file == NULL)
        printf("Unexpected error when opening file !");

    // fill output with data from LIST
    text = NULL;
    ReadList(image1, file, listHead, &text, w1, w2);
    fclose(file);

    //free list
    FreeList(listHead);
    return *(image1.copy);
}

void CharProcessNoAi(Image i, Rect r, FILE *f, ListHead *db)
{
    unsigned char resized[256];
    resize(i, r, resized);
    unsigned char carac = 0;
    long ratio = -512;
    ListImage *cur = db->head;

    //print_Array(resized, 16, 16);

    ListImage *choosen = NULL;
    while(cur)
    {

        size_t pos = 0;
        long curRatio = 0;
        int ybegin = 0;
        int xbegin = 0;
        if(cur->carac == 'l' || cur->carac == 'I')
        {
            xbegin = 3;
            ybegin = 13;
        }
        if(cur->carac == 'i')
        {
            xbegin = 2;
            ybegin = 14;
        }
        for (int y = ybegin; y < 16; ++y)
        {
            for (int x = xbegin; x < 16; ++x)
            {
                int imOk = cur->image->data[pos];
                int resOk = resized[pos];
                if (imOk && resOk)
                    curRatio++;
                else if(imOk || resOk)
                    curRatio--;
                pos++;
            }
        }
        if (curRatio > ratio)
        {
            ratio = curRatio;
            carac = cur->carac;
            choosen = cur;
        }
        cur = cur->next;
    }
    if(choosen)
        //print_Array(choosen->image->data, 16, 16);
    if(carac)
        fputc(carac, f);
    else
        fputc('?', f);
}

void ReadListNoAi(Image i, FILE *f, ListHead *l, ListHead *db)
{
    if(IsEmpty(l))
        return;
    ListChar *cur = l->head;
    while(cur != l->tail)
    {
        if(cur->type == 0)
            // char
            CharProcessNoAi(i, cur->pos, f, db);
        else if(cur->type == 1)
            // space
            fputc(' ', f);
        else if(cur->type == 2)
            // end of line
            fputc('\n', f);
        cur = cur->next;
    }
}

Image cut_noAI(char *path)
{
    //Load image and a copy
    Image image1;
    image1.data = NULL;
    image1.copy = NULL;
    load_image(path, &image1);
    Image copy;
    copy.data = NULL;
    copy.copy = NULL;
    CopyImage(image1, &copy);
    image1.copy = &copy;
    Image copy2;
    copy2.data = NULL;
    copy2.copy = NULL;
    CopyImage(image1, &copy2);
    copy.copy = &copy2;


    //Create List and fill it
    ListHead *listHead = InitListHead();
    Parse_Image2(image1, listHead);

    //Load image database
    ListHead *db = InitListHead();
    LoadImageData(db);


    //Create output file
    FILE *file = fopen("output.txt", "w+");
    if (file == NULL)
        printf("Unexpected error when opening file !");

    ReadListNoAi(image1, file, listHead, db);
    fclose(file);

    //free list
    FreeList(listHead);
    return *(image1.copy);
}
