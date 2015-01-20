SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -Iinclude
TARGETS	= stdio string stdlib

all: $(SOURCES) makesubs
	ar rcs libc.a *.o

clean: cleansubs
	-rm $(SOURCES)
	-rm *.o *.a

makesubs:
	$(foreach t,$(TARGETS),cd $(t) && make && cd ..;)

cleansubs:
	$(foreach t,$(TARGETS),cd $(t) && make clean && cd ..;)

nop:
	@@echo Nop
