SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -Iinclude
TARGETS	= stdio stat ctype string stdlib errno dirent wait unistd glob fcntl
CC	= i686-elf-gcc
AR	= i686-elf-ar
INSTALL = /home/socash/cross/lib/gcc/i686-elf/5.0.0/

all: $(SOURCES) makesubs
	$(AR) rcs libc.a *.o

clean: cleansubs
	-rm $(SOURCES)
	-rm *.o *.a

makesubs:
	$(foreach t,$(TARGETS),cd $(t) && make && cd ..;)

cleansubs:
	$(foreach t,$(TARGETS),cd $(t) && make clean && cd ..;)

install: all
	cp libc.a $(INSTALL)
	cp -R include/* $(INSTALL)/include/

nop:
	@@echo Nop
