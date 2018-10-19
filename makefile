all: OCR

OCR: main.o Bmp_Parser.o RLSA.o Xor Gui
	gcc main.o Bmp_Parser.o RLSA.o -o OCR -lSDL2 -lSDL2_image 

RLSA.o: RLSA.c
	gcc -Wall -Wextra -std=c99 -c RLSA.c

Bmp_Parser.o: Bmp_Parser.c
	gcc -Wall -Wextra -std=c99 -c Bmp_Parser.c 

main.o: main.c
	gcc -Wall -Wextra -std=c99 -c main.c

Xor: Xor.c
	gcc -Wall -Wextra -std=c99 Xor.c -o Xor -lm

Gui: gui.c
	gcc -Wall -Wextra -std=c99 `pkg-config gtk+-3.0 --cflags` gui.c -o Gui `pkg-config gtk+-3.0 --libs`

clean:
	rm *.o
	rm OCR
	rm Xor
	rm Gui
