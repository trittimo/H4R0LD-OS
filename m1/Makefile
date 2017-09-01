# Project by: Michael Trittin, RJ DeCramer, Jaron Goodman
kernel: kernel.o kernel_asm.o print.o
	ld86 -o kernel -d kernel.o kernel_asm.o print.o

kernel.o: kernel.c kernel.h
	bcc -ansi -c kernel.c -o kernel.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

print.o: print.c print.h
	bcc -ansi -c print.c -o print.o