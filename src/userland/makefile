SOURCES = hello.o userland.o common.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 
LDFLAGS	= -Tlink.ld -melf_i386
CC	= gcc
TARGET	= hello.x

all: $(SOURCES) link

link:
	ld $(LDFLAGS) -o $(TARGET) $(SOURCES)

clean:
	-rm *.o *.x
