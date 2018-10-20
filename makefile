all: OCR

OCR: main.o Bmp_Parser.o RLSA.o Xor
	gcc `pkg-config gtk+-3.0 sdl2 --cflags` Bmp_Parser.o RLSA.o main.o -o OCR `pkg-config gtk+-3.0 sdl2 --libs` -lSDL2_image

RLSA.o: RLSA.c
	gcc -Wall -Wextra -std=c99 -c RLSA.c

Bmp_Parser.o: Bmp_Parser.c
	gcc -Wall -Wextra -std=c99 -c Bmp_Parser.c 

main.o: main.c
	gcc -Wall -Wextra -std=c99 -c main.c

Xor: Xor.c
	gcc -Wall -Wextra -std=c99 Xor.c -o Xor -lm

clean:
	rm *.o;
	rm OCR;
	rm Xor
