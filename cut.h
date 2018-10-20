#ifndef CUT_H
#define CUT_H
#include <stdio.h>
#include <stdlib.h>
#include "type/image.h"

Rect CutBorder(Image image);
Image CopyImage(Image image);
void cutLine(Image image, Rect rect, Image result, FILE *f);
void CutChar(Image image, Rect line, Image result, FILE *f);
void CutCharV2(Image image, Rect line, Image result, FILE *f);
void DrawRect(Rect rect, Image image, int hor_val, int ver_val);
void DrawRect_hor(Rect rect, Image image, int val);
void DrawRect_ver(Rect rect, Image image, int val);
int GetLineThresold(Image image, Rect line);
void cutLine2(Image image, Rect rect);
void CutChar2(Image image, Rect line, Image result, FILE *f);
Image cut(char *path);
#endif
