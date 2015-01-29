ENTRY(_start)
SECTIONS
{
	/*This is the text section, note start position*/
	.text 0x40000000:
	{
		code = .; _code = .; __code = .;
		*(.text)
	}

	/*This is the data section, note 4MB alignment*/
	.data ALIGN(0x400000):
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
	}

	.bss ALIGN(0x400000):
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
	}

	end = .; _end = .; __end = .;
}
