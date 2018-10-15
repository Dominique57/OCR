#ifndef RLSA_H
#define RLSA_H

Image RLSA_v(Image image, int offset);
Image RLSA_h(Image image, int offset);
Image RLSAassemble(Image image1, Image image2);
Image RLSA_launch(Image image, int offsetv, int offseth);


#endif
