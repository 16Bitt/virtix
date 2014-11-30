#include "common.h"
#include "monitor.h"
#include "ata.h"

ushort* ata_info;

ushort disk_channel;
ushort disk_type;

void select_ata(uchar bus, uint master){
	if(bus == ATA_PRIMARY)
		if(master == ATA_MASTER)
			outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER_SEL);

		else if(master == ATA_SLAVE)
			outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_SLAVE_SEL);

		else
			PANIC("requested ATA type is neither master nor slave");
	
	else if(bus == ATA_SECONDARY)
		if(master == ATA_MASTER)
			outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER_SEL);

		else if(master == ATA_SLAVE)
			outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, ATA_SLAVE_SEL);

		else
			PANIC("requested ATA type is neither master nor slave");
	
	else
		PANIC("bad BUS for ATA");
}

uchar ident_ata(uchar bus, uchar drive){
	select_ata(bus, drive);

	ushort io = (bus == ATA_PRIMARY)? ATA_PRIMARY_IO : ATA_SECONDARY_IO;

	outb(io + ATA_REG_SECCOUNT0, 0);
	outb(io + ATA_REG_LBA0, 0);
	outb(io + ATA_REG_LBA1, 0);
	outb(io + ATA_REG_LBA2, 0);
	outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

	uchar stat = inb(io + ATA_REG_STATUS);

	if(stat){
		while((inb(io + ATA_REG_STATUS) & ATA_SR_BSY) != 0);	//Wait until not BSY
		
		stat = inb(io + ATA_REG_STATUS);

		while(!((stat = inb(io + ATA_REG_STATUS)) & ATA_SR_DRQ)) //Poll until DRQ
			if(stat & ATA_SR_ERR){
				vga_puts("WARN: ident_ata() found ERR set\n");
				return 0;
			}

		int i;
		for(i = 0; i < 256; i++)
			ata_info[i] = inw(io + ATA_REG_DATA);

		return 1; //Disk is okay
	}

	vga_puts("WARN: ident_ata() did not read valid drive\n");
	return 0;
}

void ata_wait(ushort io){
	int i;
	for(i = 0; i < 4; i++)
		(void) inb(io + ATA_REG_ALTSTATUS);
}

void init_ata(){
	ata_info = (ushort*) kmalloc(sizeof(ushort) * 256); //256 word buffer for ident()
	
	ASSERT(ident_ata(ATA_PRIMARY, ATA_MASTER));
}
