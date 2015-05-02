Virtix - Design is Law
----------------------

Virtix is an operating system with one goal in mind: simplicity. There are plenty of sacrifices, but ultimately, I think that a tiny kernel is incredibly beneficial to the programmer. With something like Linux, I don't even think Torvalds is able to fully comprehend the codebase anymore. Virtix should be able to fit within a programmers mind completely. (I don't want to bash Linux)

_Design_

* Virtix is a monolithic kernel
* The scheduler is a simple cooperative multitasker
* The main file system is a special metafs on top of FAT16, for cross-platform compatibility
* Multiboot compliant

_Layout_

* Kernel code is in `virtix/src/`
* Libc code is in `virtix/libc/`
* Userland files are in `virtix/userland`
* Userland binary sources are in `virtix/userland/src`
* Documentation will eventually be in `virtix/doc`

_Progress_

* Paging
* Multitasking
* A very simple ELF parser
* File streams
* ATA support
* A small libc
* devfs

_TODO_

* Heap implementation needs a lot of work
* ELF loader could definitely be improved

_Building Requirements_

* `nasm`
* GNU coreutils
* `i686-elf-gcc`
* GNU `make`
* `bash` or `zsh`
* `grub2` if you want to make bootable ISO images
* Optionally, `qemu` or `bochs` for testing

To build simply navigate to `virtix/` and type `make`. If you have `qemu`, and you want to test, type `make run` to build and load into the VM.

_Special Thanks To_

* James Molloy - His guide set up the basis of this kernel
* Github User Levex - His osdev repo had a lot of great examples inside
* Brokenthorn OS development series - Taught me OS dev basics/advanced x86 assembly
* Ralph Brown - His interrupt list is indispencable
* And of course, The OSDev Wiki - This is a spectacular resource
* #osdev on Freenode is a lifesaver
