CC	= i686-elf-gcc
LD	= i686-elf-ld

all:
	cd src && make
	cd libc && make
	cd userland && make
	cd userland && make disk

clean:
	cd src && make clean
	cd libc && make clean
	cd userland && make clean
	-rm *.iso *.log

run: dimg
	qemu-system-i386 -monitor stdio -net none -vga std -localtime -kernel src/build/kernel -cdrom grub.iso -drive file=userland/hdd.img,if=ide 

debug: all
	objcopy --only-keep-debug src/build/kernel src/build/ksym
	-qemu-system-i386 -vga std -localtime -kernel src/build/kernel -drive file=userland/hdd.img,if=ide -S -s & sleep 3
	(echo target remote 127.0.0.1:1234; cat) | gdb -s src/build/ksym src/build/kernel
	-pkill "qemu*"

dimg: all
	mkdir -p src/build/boot
	mkdir -p src/build/boot/grub
	cp grub.cfg src/build/boot/grub/
	cp src/build/kernel src/build/boot/kernel
	-grub-mkrescue -o grub.iso src/build

bochs: dimg
	-echo c | bochs -q

vbox: dimg
	VBoxManage convertfromraw userland/hdd.img userland/hdd.vdi --format vdi
