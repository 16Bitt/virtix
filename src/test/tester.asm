	[BITS 32]
	[ORG 0xF0000000]
	mov esp, eoc
	int 0
	jmp $
	times 1024 db 0
eoc:
