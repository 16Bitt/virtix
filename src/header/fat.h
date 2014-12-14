#ifndef FAT_H
#define FAT_H

typedef struct{
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
}__attribute__((packed)) fat_header_t;

typedef struct{
	char	name[8];
	char	extension[3];
	
	uchar	attributes;
	uchar	reserved;
	
	uchar	creation_second;
	ushort	creation_time;
	ushort	creation_date;
	ushort	accessed_date;
	
	ushort	cluster_high;
	
	ushort	mod_time;
	ushort	mod_date;
	
	ushort	cluster_low;

	uint	bytes;
}__attribute__((packed)) fat_dir_t;

extern fat_header_t fat_header;

void init_fat();
uint cluster_to_lba(uint cluster);
void disp_bpb();
void disp_fat_dir();

void* fat_load_full(char* fat_name);
fat_dir_t* fat_dir_search(char* fat_name);
char* fat_name_conv(char* actual);

void fat_write(char* name, char* buffer, uint length, bool mode);
void fat_append(char* name, char* buffer, uint length);		
fat_dir_t* fat_create(char* name);				//Find and create free entry
void fat_delete(char* name);					//Remove entry and clusters

ushort fat_next_cluster();					//First free cluster

void fat_sync();						//Sync between ram and disk tables

#define FAT_TOTAL_SIZE (fat_header.number_fats * fat_header.sectors_per_fat)
#define FAT_DIR_SIZE ((32 * fat_header.dir_size) / 512)

#define CLUSTER_BSIZE (fat_header.sectors_per_cluster * 512)
#define FAT_SPC	(fat_header.sectors_per_cluster)

#define FAT_APPEND	0
#define FAT_REPLACE	1

#endif
