#include "common.h"
#include "monitor.h"
#include "ata.h"
#include "dev.h"
#include "kheap.h"
#include "str-util.h"
#include "clock.h"

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
				////vga_puts("WARN: ident_ata() found ERR set\n");
				return 0;
			}

		int i;
		for(i = 0; i < 256; i++)
			ata_info[i] = inw(io + ATA_REG_DATA);

		return 1; //Disk is okay
	}

	////vga_puts("WARN: ident_ata() did not read valid drive\n");
	return 0;
}

void ata_wait(ushort io){
	int i;
	for(i = 0; i < 0xFF; i++)
		inb(io + ATA_REG_ALTSTATUS);
}

void ata_poll(ushort io){
	ata_wait(io);
	uchar stat;
	
	//vga_puts("ata_poll(): polling\n");

	do{
		stat = inb(io + ATA_REG_STATUS);
		
		if(stat & ATA_SR_ERR)
			PANIC("ATA driver recieved ERR while polling");
	
	} while(!(stat & ATA_SR_DRQ) && (stat & ATA_SR_BSY));
}

void ata_read_block(ushort* buffer, uint lba){
	//TODO: these are hardcoded for the primary master
	uchar cmd = 0xE0;
	ushort io = ATA_PRIMARY_IO;
	uchar drive = ATA_MASTER;

	//Preliminary disk setup
	outb(io + ATA_REG_HDDEVSEL, (cmd | (uchar) (lba >> 24 & 0x0F)));	//Select block
	outb(io + ATA_REG_ERROR, 0);	//Clear error register
	outb(io + ATA_REG_SECCOUNT0, 1);	//Read one sector
	
	outb(io + ATA_REG_LBA0, (uchar) lba & 0xFF); //Lower 8 bits of lba
	outb(io + ATA_REG_LBA1, (uchar) ((lba >> 8) & 0xFF)); //Middle 8 bits of lba
	outb(io + ATA_REG_LBA2, (uchar) ((lba >> 16) & 0xFF)); //Upper 8 bits of lba
	
	//Send commands
	outb(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	ata_poll(io);

	int i;
	for(i = 0; i < 256; i++){
		ushort data = inw(io + ATA_REG_DATA);
		buffer[i] = data;
	}

	ata_wait(io);
}

void ata_read_blocks(ushort* buffer, uint lba, uint length){
	while(length != 0){
		ata_read_block(buffer, lba);
		buffer = (ushort*) ((uint) buffer + 512);
		lba++;
		length--;
	}
}

void ata_write_block(ushort* buffer, uint lba){
	//TODO: these are hardcoded for the primary master
	uchar cmd = 0xE0;
	ushort io = ATA_PRIMARY_IO;
	uchar drive = ATA_MASTER;

	//Flush the disk cache (necessary for certain hardware setups)
	outb(io + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);
	ata_wait(io);
	outb(io + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH_EXT);
	ata_poll(io);

	//Preliminary disk setup
	outb(io + ATA_REG_HDDEVSEL, (cmd | (uchar) (lba >> 24 & 0x0F)));	//Select block
	outb(io + ATA_REG_ERROR, 0);	//Clear error register
	outb(io + ATA_REG_SECCOUNT0, 1);	//Read one sector
	
	outb(io + ATA_REG_LBA0, (uchar) lba & 0xFF); //Lower 8 bits of lba
	outb(io + ATA_REG_LBA1, (uchar) ((lba >> 8) & 0xFF)); //Middle 8 bits of lba
	outb(io + ATA_REG_LBA2, (uchar) ((lba >> 16) & 0xFF)); //Upper 8 bits of lba

	//Send commands
	outb(io + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
	ata_poll(io);

	int i;
	for(i = 0; i < 256; i++)
		outw(io + ATA_REG_DATA, buffer[i]);

	ata_wait(io);
}

void ata_write_blocks(ushort* buffer, uint lba, uint length){
	while(length != 0){
		ata_write_block(buffer, lba);
		buffer = (ushort*) ((uint) buffer + 512);
		lba++;
		length--;
	}
}

void init_ata(){
	ata_info = (ushort*) kmalloc(sizeof(ushort) * 256); //256 word buffer for ident()
	
	dev_t* dev = (dev_t*) kmalloc(sizeof(dev_t));
	dev->drive = ATA_MASTER;
	dev->bus = ATA_PRIMARY;
	strmov(dev->name, "hdd");

	//TODO: don't hardcode root disk
	ASSERT(ident_ata(ATA_PRIMARY, ATA_MASTER));
	ASSERT(ident_ata(dev->bus, dev->drive));

	disk_channel = ATA_PRIMARY;
	disk_type = ATA_MASTER;
}

void ata_read_drv(dev_t* dev, ushort* buffer, uint lba){
	select_ata(dev->bus, dev->drive);
	
	uchar cmd;
	if(dev->drive == ATA_MASTER)
		cmd = 0xE0;
	else
		cmd = 0xF0;
	
	ushort io;
	if(dev->drive == ATA_PRIMARY)
		io = ATA_PRIMARY_IO;
	else
		io = ATA_SECONDARY_IO;

	//Preliminary disk setup
	outb(io + ATA_REG_HDDEVSEL, (cmd | (uchar) (lba >> 24 & 0x0F)));	//Select block
	outb(io + ATA_REG_ERROR, 0);	//Clear error register
	outb(io + ATA_REG_SECCOUNT0, 1);	//Read one sector
	
	outb(io + ATA_REG_LBA0, (uchar) lba & 0xFF); //Lower 8 bits of lba
	outb(io + ATA_REG_LBA1, (uchar) ((lba >> 8) & 0xFF)); //Middle 8 bits of lba
	outb(io + ATA_REG_LBA2, (uchar) ((lba >> 16) & 0xFF)); //Upper 8 bits of lba
	
	//Send commands
	outb(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	ata_poll(io);

	int i;
	for(i = 0; i < 256; i++){
		ushort data = inw(io + ATA_REG_DATA);
		buffer[i] = data;
	}

	ata_wait(io);
}

void ata_write_drv(dev_t* dev, ushort* buffer, uint lba){
	select_ata(dev->bus, dev->drive);
	
	uchar cmd;
	if(dev->drive == ATA_MASTER)
		cmd = 0xE0;
	else
		cmd = 0xF0;
	
	ushort io;
	if(dev->drive == ATA_PRIMARY)
		io = ATA_PRIMARY_IO;
	else
		io = ATA_SECONDARY_IO;
	
	//Flush the disk cache (necessary for certain hardware setups)
	outb(io + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);
	ata_wait(io);
	outb(io + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH_EXT);
	ata_poll(io);

	//Preliminary disk setup
	outb(io + ATA_REG_HDDEVSEL, (cmd | (uchar) (lba >> 24 & 0x0F)));	//Select block
	outb(io + ATA_REG_ERROR, 0);	//Clear error register
	outb(io + ATA_REG_SECCOUNT0, 1);	//Read one sector
	
	outb(io + ATA_REG_LBA0, (uchar) lba & 0xFF); //Lower 8 bits of lba
	outb(io + ATA_REG_LBA1, (uchar) ((lba >> 8) & 0xFF)); //Middle 8 bits of lba
	outb(io + ATA_REG_LBA2, (uchar) ((lba >> 16) & 0xFF)); //Upper 8 bits of lba

	//Send commands
	outb(io + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);
	ata_poll(io);

	int i;
	for(i = 0; i < 256; i++)
		outw(io + ATA_REG_DATA, buffer[i]);

	ata_wait(io);
}
