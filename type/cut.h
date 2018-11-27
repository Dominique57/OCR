#ifndef CUT_H
#define CUT_H
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

Rect CutBorder(Image image);
Image CopyImage(Image image);
void cutLine(Image image, Rect rect, Image result, FILE *f, float *w1, float *w2);
void CutChar(Image image, Rect line, Image result, FILE *f);
void CutChar2(Image image, Rect line, Image result, FILE *f, float *w1, float *w2);
void CharProcess(Image image, Rect rect, FILE *f, float *w1, float *w2);
void DrawRect(Rect rect, Image image, int hor_val, int ver_val);
void DrawRect_hor(Rect rect, Image image, int val);
void DrawRect_ver(Rect rect, Image image, int val);
int GetLineThresold(Image image, Rect line);
Image cut(char *path);
#endif
