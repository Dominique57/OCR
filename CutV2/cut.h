#ifndef CUT_H
#define CUT_H
#include "../type/image.h"

Rect CutBorder(Image image);
Image CopyImage(Image image);
void cutLine(Image image, Rect rect, Image result);
void CutChar(Image image, Rect line, Image result);
void DrawRect(Rect rect, Image image, int hor_val, int ver_val);
void DrawRect_hor(Rect rect, Image image, int val);
void DrawRect_ver(Rect rect, Image image, int val);

#endif
