SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -Iinclude
TARGETS	= stdio ctype string stdlib errno dirent wait unistd glob
CC	= i686-elf-gcc
AR	= i686-elf-ar

all: $(SOURCES) makesubs
	$(AR) rcs libc.a *.o

clean: cleansubs
	-rm $(SOURCES)
	-rm *.o *.a

makesubs:
	$(foreach t,$(TARGETS),cd $(t) && make && cd ..;)

cleansubs:
	$(foreach t,$(TARGETS),cd $(t) && make clean && cd ..;)

nop:
	@@echo Nop
