# Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
kernel: kernel.o kernel_asm.o io.o math.o interrupt.o
	ld86 -o kernel -d kernel.o kernel_asm.o io.o math.o interrupt.o

kernel.o: kernel.c kernel.h
	bcc -ansi -c kernel.c -o kernel.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

io.o: io.c io.h
	bcc -ansi -c io.c -o io.o

math.o: math.c math.h
	bcc -ansi -c math.c -o math.o

interrupt.o: interrupt.c interrupt.h
	bcc -ansi -c interrupt.c -o interrupt.o

clean:
	rm -f *.o