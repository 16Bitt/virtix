CFLAGS	= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
OUTPUT	= libc.a
SOURCES	= stdlib/stdlib.o stdio/stdio.o string/string.o libc_init.o

all: $(SOURCES)
	ar cr $(OUTPUT) $(SOURCES)

clean:
	-rm $(SOURCES) $(OUTPUT)

$(SOURCES): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@
