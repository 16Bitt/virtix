	[BITS 32]
	[ORG 1024]
	int 30
	mov [0xffffffff], eax
	jmp $
