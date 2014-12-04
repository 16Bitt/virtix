#include "common.h"
#include "monitor.h"
#include "fat.h"
#include "ata.h"
#include "kheap.h"

fat_header_t	fat_header;
ushort*		cluster_list;
fat_dir_t*	fat_dir;

void init_fat(){
	ushort* mbr = (ushort*) kmalloc(512);
	ata_read_block(mbr, 0);
	memcpy(&fat_header, mbr, sizeof(fat_header_t));
	kfree(mbr);

	cluster_list = (ushort*) kmalloc(fat_header.sectors_per_fat * 512);
	ata_read_blocks(cluster_list, fat_header.number_reserved, fat_header.sectors_per_fat);

	fat_dir = (fat_dir_t*) kmalloc(sizeof(fat_dir_t) * fat_header.dir_size);
	ata_read_blocks((ushort*) fat_dir, fat_header.number_reserved + FAT_TOTAL_SIZE, FAT_DIR_SIZE);

	ASSERT(((fat_header.dir_size * 32) % 512) == 0);
}

uint cluster_to_lba(uint cluster){
	uint sector = cluster * fat_header.sectors_per_cluster - (2 * fat_header.sectors_per_cluster);
	return sector + FAT_TOTAL_SIZE + fat_header.number_reserved + FAT_DIR_SIZE;
}

fat_dir_t* fat_dir_search(char* fat_name){
	int i;
	for(i = 0; i < fat_header.dir_size; i++){
		fat_dir[i].attributes = 0;
		if(strcmp(fat_name, fat_dir[i].name) == 0)
			return &fat_dir[i];
	}

	PANIC("internal search for file in FAT failed");
}

void* fat_load_full(char* fat_name){
	fat_dir_t* head = fat_dir_search(fat_name);
	
	uint bytes = head->bytes;
	if(bytes % (512 * fat_header.sectors_per_cluster))
		bytes += fat_header.sectors_per_cluster * 512;

	void* file = kmalloc_a(bytes);
	void* top = file;		//Save the top of the file
	ushort cluster = head->cluster_low;
	
	while(cluster < 0xFFF0){
		ata_read_blocks(file, cluster_to_lba(cluster), fat_header.sectors_per_cluster);
		file = (void*) ((uint) file + (512 * fat_header.sectors_per_cluster));
		cluster = cluster_list[cluster];
	}

	return top;
}

void disp_bpb(){
	vga_puts("init_fat(): read FAT12/16 volume\n");
	
	vga_puts("\t* with OEM identifier '");
	int i;
	for(i = 0; i < 8; i++)
		vga_putc(fat_header.oem_ident[i]);
	
	vga_puts("'\n\t* with ");
	vga_puts_hex(fat_header.sectors_per_cluster);
	vga_puts(" sectors per cluster\n");

	vga_puts("\t* with ");
	vga_puts_hex(fat_header.number_fats);
	vga_puts(" FATs\n");

	vga_puts("\t* with ");
	vga_puts_hex(fat_header.number_reserved);
	vga_puts(" reserved sectors\n");
	
	vga_puts("\t* with ");
	vga_puts_hex(fat_header.media_type);
	vga_puts(" media type\n");
	
	vga_puts("\t* with ");
	vga_puts_hex(fat_header.number_hidden);
	vga_puts(" hidden sectors\n");

	vga_puts("\t* with ");
	vga_puts_hex(fat_header.dir_size);
	vga_puts(" root entries\n");
}

void disp_fat_dir(){
	vga_puts("init_fat(): FAT volume contains the following files\n");
	
	int i;
	for(i = 0; i < fat_header.dir_size; i++){
		if(fat_dir[i].cluster_low != 0){
			vga_puts("\t* ");
			vga_puts(fat_dir[i].name);
			vga_puts("\n");
		}
	}
}
