# Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
kernel: kernel.o kernel_asm.o io.o math.o interrupt.o
	ld86 -o kernel -d kernel.o kernel_asm.o io.o math.o interrupt.o

shell: shell.o lib.o string.o mem.o
	ld86 -o shell -d shell.o lib.o string.o mem.o

kernel.o: kernel.c kernel.h
	bcc -ansi -c kernel.c -o kernel.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

shell.o: shell.c shell.h
	bcc -ansi -c shell.c -o shell.o

string.o: string.c string.h
	bcc -ansi -c string.c -o string.o

mem.o: mem.c mem.h
	bcc -ansi -c mem.c -o mem.o

lib.o: lib.asm
	as86 lib.asm -o lib.o

io.o: io.c io.h
	bcc -ansi -c io.c -o io.o

math.o: math.c math.h
	bcc -ansi -c math.c -o math.o

interrupt.o: interrupt.c interrupt.h
	bcc -ansi -c interrupt.c -o interrupt.o

loadFile: loadFile.c
	gcc loadFile.c -o loadFile

all: loadFile kernel shell

clean:
	rm -f *.o