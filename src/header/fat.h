#ifndef FAT_H
#define FAT_H

struct fat_header{
	uchar	jmp[3];			//jmp short NOP
	uchar	oem_ident[8];		//Short OEM string
	
	ushort	bytes_per_sector;
	uchar	sectors_per_cluster;
	
	ushort	number_reserved;	//The reserved sectors for boot
	uchar	number_fats;		//Usually two
	ushort	dir_size;
	
	ushort	short_num_sectors;	//If zero, need to check other value
	uchar	media_type;
	ushort	sectors_per_fat;
	ushort	sectors_per_track;

	ushort	number_heads;
	uint	number_hidden;
	uint	long_num_sectors;	//Used if the other value is zero
} __attribute__ ((packed));

typedef struct fat_header fat_header_t;

#endif
