Virtix - Design is Law
----------------------

Virtix is an operating system with one goal in mind: simplicity. There are plenty of sacrifices, but ultimately, I think that a tiny kernel is incredibly beneficial to the programmer. With something like Linux, I don't even think Torvalds is able to fully comprehend the codebase anymore. Virtix should be able to fit within a programmers mind completely. (I don't want to bash Linux)

_Design_

* Virtix is a monolithic kernel, with no module support. Once again, simplicity
* The scheduler is a simple cooperative multitasker
* The main file system is FAT16, for cross-platform compatibility
* Made with GRUB in mind
* Will eventually have a graphical mode, made for VESA

_Progress_

* As of now, there is a simple paging implementation
* A very simple ELF parser
* File streams

_TODO_

* Heap implementation needs a lot of work
* ELF loader could definitely be improved
* Paging needs a rewrite
* Scheduler still needs a lot of work

_Building Requirements_

* `nasm`
* GNU coreutils
* `gcc`
* `ruby` (2.0+)
* GNU `make`
* `bash` or `zsh`
* Optionally, `qemu` for running immedeatly

To build simply navigate to `virtix/src/` and type `make`. If you have `qemu`, and you want to test, type `make run` to build and load into the VM.

_Special Thanks To_

* James Molloy - His guide set up the basis of this kernel
* Github User Levex - His osdev repo had a lot of great examples inside
* Brokenthorn OS development series - Taught me OS dev basics/advanced x86 assembly
* Ralph Brown - His interrupt list is indispencable
* And of course, The OSDev Wiki - This is a spectacular resource

_Update_

For the sake of time, multitasking is being put on hold. I will add it later, but as of typing this, my brain has had 10000000 too many triple faults for one night. I will return to this later on.
