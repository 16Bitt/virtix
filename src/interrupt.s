	[EXTERN istack_base]
	[EXTERN stack_hold]

%macro ISR_BLANK 1
		[GLOBAL isr%1]
	isr%1:
		cli
		push byte 0
		push byte %1
		jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
		[GLOBAL isr%1]
	isr%1:
		push byte %1
		jmp isr_common_stub
%endmacro

ISR_BLANK 0
ISR_BLANK 1
ISR_BLANK 2
ISR_BLANK 3
ISR_BLANK 4
ISR_BLANK 5
ISR_BLANK 6
ISR_BLANK 7
ISR_ERR 8
ISR_BLANK 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_BLANK 15
ISR_BLANK 16
ISR_BLANK 17
ISR_BLANK 18
ISR_BLANK 19
ISR_BLANK 20
ISR_BLANK 21
ISR_BLANK 22
ISR_BLANK 23
ISR_BLANK 24
ISR_BLANK 25
ISR_BLANK 26
ISR_BLANK 27
ISR_BLANK 28
ISR_BLANK 29
ISR_BLANK 30
ISR_BLANK 31

ISR_BLANK 32
ISR_BLANK 33
ISR_BLANK 34
ISR_BLANK 35
ISR_BLANK 36
ISR_BLANK 37
ISR_BLANK 38
ISR_BLANK 39
ISR_BLANK 40
ISR_BLANK 41
ISR_BLANK 42
ISR_BLANK 43
ISR_BLANK 44
ISR_BLANK 45
ISR_BLANK 46
ISR_BLANK 47
ISR_BLANK 48

ISR_BLANK 0x80

	[EXTERN isr_handler]
	[EXTERN stack_hold]

isr_common_stub:
	pusha			;REGS
	mov ax, ds
	push eax		;REGS, EAX

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp		;REGS, EAX, ESP

	call isr_handler
	
	add esp, 4		;REGS, EAX
	pop ebx			;REGS
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa			;--
	add esp, 8
	iret

%macro IRQ 2
		[GLOBAL irq%1]
	
	irq%1:
		;mov [stack_hold], esp
		;mov esp, [istack_base]
		push byte 0
		push byte %2
		jmp isr_common_stub
%endmacro
	
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
