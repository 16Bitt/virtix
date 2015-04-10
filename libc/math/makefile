SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -I../include
CC	= i686-elf-gcc

all: copyup

clean:
	-rm $(SOURCES)

copyup: $(SOURCES)
	cp $(SOURCES) ../

nop:
	@@echo Nop
