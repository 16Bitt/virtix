SOURCES = userland.o
CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iheader/ 

all: $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o userland.x
