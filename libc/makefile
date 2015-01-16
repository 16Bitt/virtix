SOURCES	= crt0.o
TARGETS	= stdio string stdlib

all: $(SOURCES) makesubs

makesubs:
	$(foreach target,$(TARGETS),cd $(target) && make && cd ..;)

clean: cleansubs
	rm $(SOURCES)
	-rm *.o

cleansubs:
	$(foreach target,$(TARGETS),cd $(target) && make clean && cd -;)
