#ifndef DESC_TABLE_H
#define DESC_TABLE_H

struct gdt_entry_struct{
	unsigned short limit_low;
	unsigned short base_low;
	u8int base_middle;
	u8int access;
	u8int granularity;
	u8int base_high;
}__attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

struct idt_entry_struct{
	unsigned short base_lo;
	unsigned short sel;
	u8int always0;
	u8int flags;
	unsigned short base_hi;
}__attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// A struct describing a Task State Segment.
struct tss_entry_struct
{
    unsigned int prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
    unsigned int esp0;       // The stack pointer to load when we change to kernel mode.
    unsigned int ss0;        // The stack segment to load when we change to kernel mode.
    unsigned int esp1;       // Unused...
    unsigned int ss1;
    unsigned int esp2;  
    unsigned int ss2;   
    unsigned int cr3;   
    unsigned int eip;   
    unsigned int eflags;
    unsigned int eax;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
    unsigned int es;         // The value to load into ES when we change to kernel mode.
    unsigned int cs;         // The value to load into CS when we change to kernel mode.
    unsigned int ss;         // The value to load into SS when we change to kernel mode.
    unsigned int ds;         // The value to load into DS when we change to kernel mode.
    unsigned int fs;         // The value to load into FS when we change to kernel mode.
    unsigned int gs;         // The value to load into GS when we change to kernel mode.
    unsigned int ldt;        // Unused...
    unsigned short trap;
    unsigned short iomap_base;

} __attribute__((packed));

typedef struct tss_entry_struct tss_t;


void init_descriptor_tables();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr0x80();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void set_kernel_stack(unsigned int stack);

#endif
