SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -I../include

all: copyup

clean:
	-rm $(SOURCES)

copyup: $(SOURCES)
	cp $(SOURCES) ../

nop:
	@@echo Nop
