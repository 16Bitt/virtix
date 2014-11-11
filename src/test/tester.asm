	[BITS 32]
	[ORG 1024]
	int 69
	mov [0xffffffff], eax
	jmp $
