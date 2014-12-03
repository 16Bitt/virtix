#include "common.h"
#include "monitor.h"
#include "fat.h"
#include "ata.h"
#include "kheap.h"

fat_header_t fat_header;
ushort* cluster_list;

void init_fat(){
	ushort* mbr = (ushort*) kmalloc(512);
	ata_read_block(mbr, 0);
	memcpy(&fat_header, mbr, sizeof(fat_header_t));
	kfree(mbr);

	ushort* fat = (ushort*) kmalloc(fat_header->sectors_per_fat);

	ASSERT(((fat_header->dir_size * 32) % 512) == 0);
}

uint cluster_to_lba(uint cluster){
	uint sector = cluster / fat_header->sectors_per_cluster;
	return sector + (fat_header->sectors_per_fat * fat_header->number_fats) + fat_header->number_reserved + ((32 * fat_header) / 512);
}
