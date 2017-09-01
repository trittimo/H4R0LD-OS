# Project by: Michael Trittin, RJ DeCramer, Jaron Goodman

LD=ld86
CC=bcc
CCFLAGS=-ansi -O -c -I./lib
LDFLAGS=
AS=as86

kernel: kernel.o kernel_asm.o math.o string.o mem.o
	$(LD) -o kernel -d kernel.o kernel_asm.o math.o string.o mem.o $(LDFLAGS)

shell: shell.o lib.o string.o mem.o math.o
	$(LD) -o shell -d shell.o lib.o string.o mem.o math.o $(LDFLAGS)

kernel.o: src/kernel/kernel.c src/kernel/kernel.h
	$(CC) src/kernel/kernel.c -o kernel.o $(CCFLAGS)

kernel_asm.o: asm/kernel.asm
	$(AS) asm/kernel.asm -o kernel_asm.o

shell.o: src/shell/shell.c src/shell/shell.h
	$(CC) src/shell/shell.c -o shell.o $(CCFLAGS)

string.o: lib/string.c lib/string.h
	$(CC) lib/string.c -o string.o $(CCFLAGS)

mem.o: lib/mem.c lib/mem.h
	$(CC) lib/mem.c -o mem.o $(CCFLAGS)

lib.o: asm/lib.asm
	$(AS) asm/lib.asm -o lib.o

math.o: lib/math.c lib/math.h
	$(CC) lib/math.c -o math.o $(CCFLAGS)

loadFile: util/loadFile.c
	gcc util/loadFile.c -o loadFile

floppya:
	cp asm/bootload.asm .
	nasm bootload.asm
	rm bootload.asm
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=blob/map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
	dd if=blob/dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
	cp blob/* .
	./loadFile message.txt
	./loadFile tstprg
	./loadFile tstpr2
	./loadFile shell
	./loadFile phello
	./loadFile help
	rm message.txt tstprg tstpr2

all: loadFile kernel shell floppya

clean:
	rm -f *.o bochsout.txt bootload kernel floppya.img copy.txt snapshot.txt loadFile shell linter dir.img map.img help phello kernel.asm lib.asm