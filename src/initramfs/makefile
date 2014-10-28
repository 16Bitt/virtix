all:
	echo This is a test > text-file.o
	./gen_rd.rb *.o *.x
	./as_rd.sh
	rm *.o *.x
	rm *.asm
