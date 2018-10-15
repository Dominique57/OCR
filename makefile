all: OCR

OCR: main.o Bmp_Parser.o RLSA.o 
	gcc main.o Bmp_Parser.o RLSA.o -o OCR -lSDL2 -lSDL2_image 

RLSA.o: RLSA.c
	gcc -Wall -Wextra -std=c99 -c RLSA.c

Bmp_Parser.o: Bmp_Parser.c
	gcc -Wall -Wextra -std=c99 -c Bmp_Parser.c 

main.o: main.c
	gcc -Wall -Wextra -std=c99 -c main.c



