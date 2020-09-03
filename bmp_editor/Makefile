
.PHONY: clean all
    
FLAGS = -Wall -Wextra -Werror -Iinclude -g 

EXE = hw_01
OBJECT_FILES = obj/bmp.o obj/stego.o obj/main.o

all: $(EXE)


$(EXE): $(OBJECT_FILES)
	gcc $(OBJECT_FILES) -o $(EXE)

obj:
	mkdir obj

obj/main.o: src/main.c include/bmp.h include/stego.h | obj 
	gcc -c  src/main.c -o obj/main.o $(FLAGS)

obj/bmp.o: src/bmp.c include/bmp.h | obj 
	gcc -c  src/bmp.c -o obj/bmp.o $(FLAGS)

obj/stego.o: src/stego.c include/stego.h include/bmp.h | obj 
	gcc -c  src/stego.c -o obj/stego.o $(FLAGS)	

clean:
	rm -rf obj $(EXE)
