/*Linker file for the kernel
Taken from "Bran's Kernel Development"*/

ENTRY(start)
SECTIONS
{
	/*Executable code*/
	.text 0x1000000 :
	{
		code = .; _code = .; __code = .;
		*(.text)
		. = ALIGN(4096);
	}
	
	/*Data and read only data*/
	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	
	/*Zero initialized data*/
	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	
	end = .; _end = .; __end = .;
}
