all:
	cd src && make
	cd libc && make
	cd userland && make

clean:
	cd src && make clean
	cd libc && make clean
	cd userland && make clean
	-rm *.iso *.log

run: all dimg
	qemu-system-i386 -kernel src/build/kernel -cdrom grub.iso -drive file=userland/hdd.img,if=ide

debug: all
	objcopy --only-keep-debug src/build/kernel build/ksym
	qemu-system-x86_64 -nographic -no-reboot -kernel src/build/kernel -initrd initramfs/initrd -s -S & sleep 3
	(echo target remote 127.0.0.1:1234; cat) | gdb -s build/ksym src/build/kernel
	-pkill "qemu*"

dimg: all
	mkdir -p src/build/boot
	mkdir -p src/build/boot/grub
	cp grub.cfg src/build/boot/grub/
	cp src/build/kernel src/build/boot/kernel
	cp src/initramfs/initrd src/build/boot/initrd
	-grub-mkrescue -o grub.iso src/build

bochs: dimg
	-echo c | bochs -q 2> bochs.log

vbox: dimg
	VBoxManage convertfromraw userland/hdd.img userland/hdd.vdi --format vdi
