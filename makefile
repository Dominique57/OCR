all: OCR

OCR: main.o Bmp_Parser.o 
	gcc main.o Bmp_Parser.o -o OCR

Bmp_Parser.o: Bmp_Parser.c
	gcc -Wall -Wextra -std=c99 -c Bmp_Parser.c -lSDL2 -lSDL2_image

main.o: main.c
	gcc -Wall -Wextra -std=c99 -c main.c



