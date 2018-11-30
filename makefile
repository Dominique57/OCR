CFLAGS = -Wall -Wextra -std=c99

all: OCR

OCR: main.o Bmp_Parser.o cut.o resize.o NN.o List.o
	gcc `pkg-config --cflags gtk+-3.0` *.o -o OCR `pkg-config --libs gtk+-3.0 ` -lm -lSDL2 -lSDL2_image

main.o: main.c
	gcc -c main.c `pkg-config --cflags --libs gtk+-3.0`

Bmp_Parser.o: Bmp_Parser.c
	gcc -c Bmp_Parser.c

cut.o: cut.c
	gcc -c cut.c

resize.o: resize.c
	gcc -c resize.c

NN.o: NN.c
	gcc -O3 -c NN.c

List.o: List.c
	gcc -c List.c


clean:
	rm *.o;
	rm OCR;
