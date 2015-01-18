SOURCES = $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS	= -fno-builtin -nostdinc -nostdlib -m32 -Iinclude
TARGETS	= stdio string stdlib

all: $(SOURCES) makesubs

clean: cleansubs
	-rm $(SOURCES)

makesubs:
	$(foreach t,$(TARGETS),cd $(t) && make && cd ..;)

cleansubs:
	$(foreach t,$(TARGETS),cd $(t) && make clean && cd ..;)

nop:
	@@echo Nop
