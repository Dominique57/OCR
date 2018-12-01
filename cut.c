#include <stdio.h>
#include <stdlib.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "type/cut.h"
#include "resize.h"
#include "NN.h"
#include <time.h>



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

    dest->data = malloc(arrSize * sizeof(unsigned char));
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
void FreeImage(Image image)
{
    if(image.data)
        free(image.data);
    if(image.copy)
        FreeImage(*(image.copy));
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

            CutChar2(image, inrect, list);
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

        CutChar2(image, inrect, list);
        ListChar *listChar = InitListChar();
        listChar->type = 2;
        CopyRect(inrect, &(listChar->pos));
        AddListChar(list, listChar);
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
                DrawRect_ver(charPos, *(image.copy), 2);
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
        DrawRect_ver(charPos, *(image.copy), 2);

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
        }while (carac == ' ');

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

Image cut_new(char *path, char *text, int learningIteration)
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

    //load NN
    float w1[nbInput * nbHidden + nbHidden];
    float w2[nbHidden * nbOutput + nbOutput];

    //if file does not exist, uncomment, run and comment following lines
    //Initialization(w1, w2, 0);
    //SaveNetwork(w1, w2);
    Initialization(w1, w2, 1);


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
        if(*textCur == NULL || **textCur != '\0')
        {
            printf("TEXTE CORRESPOND PAS A LA DETECTION DE L'IMAGE!\n");
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
