#ifndef FAT32_H
#define FAT32_H

#include "common.h"

struct fat_dir{
	char name[8];
	char ext[3];
	uchar file_attrib;
	uchar user_attrib;

	char undelete;
	ushort create_time;
	ushort create_date;
	ushort access_date;

	ushort cluster_high;

	ushort mod_time;
	ushort mod_date;

	ushort cluster_low;
	uint file_size;

} __attribute__ ((packed));

typedef struct fat_dir fat_dir_t;

typedef struct{
	ushort	bytes_per_sector;
	uchar	sectors_per_cluster;
	ushort	reserved_sectors;
	
	uchar	number_of_fats;
	ushort	number_of_dirs;
	uint	total_sectors;

	uint	hidden_sectors;
	uint	fat_sector_size;
	ushort	root_dir_cluster;

	ushort	boot_backup_cluster;
	ushort	first_usable_cluster;
} fat_t;

uint cluster_to_lba(uint cluster);

#endif
