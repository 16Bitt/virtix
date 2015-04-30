#include "common.h"
#include "monitor.h"
#include "fat.h"
#include "ata.h"
#include "kheap.h"
#include "dev.h"

fat_header_t	fat_header;
ushort*		cluster_list;
fat_dir_t*	fat_dir;
dev_t*		fat_dev;

int fat_strcmp(char* str1, char* str2){
	int i;
	for(i = 0; i < 11; i++)
		if(str1[i] != str2[i])
			return -1;

	return 0;
}

void init_fat(){
	ushort* mbr = (ushort*) kmalloc(512);
	ata_read_block(mbr, 0);
	memcpy(&fat_header, mbr, sizeof(fat_header_t));
	kfree(mbr);

	cluster_list = (ushort*) kmalloc(fat_header.sectors_per_fat * 512);
	ata_read_blocks(cluster_list, fat_header.number_reserved, fat_header.sectors_per_fat);

	fat_dir = (fat_dir_t*) kmalloc(32 * fat_header.dir_size);
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
		if(fat_strcmp(fat_dir[i].name, fat_name) == 0)
			return &fat_dir[i];
	}

	PANIC("internal search for file in FAT failed");
}

fat_dir_t* fat_search(char* fat_name){
	int i;

	for(i = 0; i < fat_header.dir_size; i++){
		fat_dir[i].attributes = 0;
		if(strcmp(fat_name, fat_dir[i].name) == 0)
			return &fat_dir[i];
	}

	return NULL;
}

void* fat_load_full(char* fat_name){
	fat_dir_t* head = fat_dir_search(fat_name);
	
	uint bytes = head->bytes;
	if(bytes % (512 * fat_header.sectors_per_cluster))
		bytes += fat_header.sectors_per_cluster * 512;

	void* file = kmalloc(bytes);
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
	
	int count = 0;

	int i;
	for(i = 0; i < fat_header.dir_size; i++){
		if(fat_dir[i].name[0] != 0){
			vga_fmt("\t* %s\t%d bytes\tfirst cluster %d\n", fat_dir[i].name, fat_dir[i].bytes, fat_dir[i].cluster_low);
			count++;
		}
	}

	vga_fmt("\nFAT directory contains %d files.\n", count);
}

char hold_name[12];

char* fat_name_conv(char* actual){
	char* internal = (char*) kmalloc(12); //11 byte file name
	memset(hold_name, ' ', 11);

	int i; bool spc; int len = 0;
	for(i = 0; i < 11; i++, len++){
		if(actual[len] == 0)
			break;

		if(actual[len] == '.')
			i = 7;
		else
			hold_name[i] = actual[len];
	}
	
	return hold_name;
}

fat_dir_t* fat_create(char* name){
	name = fat_name_conv(name);
	fat_dir_t* target = NULL;
	
	if(fat_search(name)){
		return NULL;
	}
	
	int i;
	for(i = 0; i < fat_header.dir_size; i++)
		if(fat_dir[i].name[0] == 0){
			target = &fat_dir[i];
			break;
		}

	if(target == NULL)
		PANIC("internal search for free FAT entry failed");

	//Create initialized entry
	memcpy(target->name, name, 11);
	target->cluster_low = 0xFFFF;
	target->bytes = 0;
	
	NOTIFY("making FAT file");
	return target;
}

ushort fat_next_cluster(){
	int i;
	for(i = 2; i < (fat_header.sectors_per_fat * 512); i++)
		if(cluster_list[i] == 0)
			return i;

	PANIC("FAT driver could find no free clusters");
	return 0;
}

void fat_delete(char* name){
	name = fat_name_conv(name);

	int i;
	for(i = 0; i < fat_header.dir_size; i++)
		if(fat_strcmp(name, fat_dir[i].name) == 0){
			ushort cluster = fat_dir[i].cluster_low;
			while(cluster < 0xFFF0){
				ushort next = cluster_list[cluster];
				cluster_list[cluster] = 0;
				cluster = next;
			}

			memset(&fat_dir[i], 0, sizeof(fat_dir_t));
			break;
		}

}

uint fat_read_block(char* name, uint offset, uchar* buffer){
	name = fat_name_conv(name);
	fat_dir_t* entry = fat_search(name);

	if(entry == NULL)
		return (uint) -1;

	ushort cluster = entry->cluster_low;
	if(cluster >= 0xFFF0)
		return (uint) -1;
	
	while(offset != 0){
		cluster = cluster_list[cluster];
		if(cluster >= 0xFFF0)
			return (uint) -1;
		
		offset--;
	}

	ata_read_blocks((ushort*) buffer, cluster_to_lba(cluster), FAT_SPC);
	return 0;
}

uint fat_write_block(char* name, uint offset, uchar* buffer){
	name = fat_name_conv(name);
	fat_dir_t* entry = fat_search(name);
	
	if(entry == NULL)
		return (uint) -1;

	ushort cluster = entry->cluster_low;
	if(cluster >= 0xFFF0){	//If the cluster is the end, make a new one
		cluster = fat_next_cluster();
		entry->cluster_low = cluster;
		cluster_list[cluster] = 0xFFFF;
	}

	while(offset != 0){
		if(cluster_list[cluster] >= 0xFFF0){ //End? Make a new one and continue
			cluster_list[cluster] = fat_next_cluster();
			cluster_list[cluster_list[cluster]] = 0xFFFF;
		}
		
		cluster = cluster_list[cluster];
		offset--;
	}
	
	ata_write_blocks((ushort*) buffer, cluster_to_lba(cluster), FAT_SPC);
	return 0;
}

void update_size(fat_dir_t* entry, uint offset, uint length){
	if(offset > entry->bytes){
		entry->bytes = offset + length;
		return;
	}
	
	if(offset + length > entry->bytes){
		entry->bytes = offset + length;
	}

	return;
}

uint fat_cluster_size(fat_dir_t* entry){
	uint size = 0;
	uint cluster = entry->cluster_low;

	while(cluster < 0xFFF0){
		cluster = cluster_list[cluster];
		size++;
	}

	return size;
}

uint offset_to_cluster(uint offset){
	return offset / CLUSTER_BSIZE;
}

uint offset_in_cluster(uint offset){
	return offset - offset_to_cluster(offset);
}

uint fat_read(char* name, uint offset, uint length, char* buffer){
	char* fname = fat_name_conv(name);
	fat_dir_t* entry = fat_search(fname);
	
	if(entry == NULL)
		return (uint) -1;
	
	uchar* scratch = (uchar*) kmalloc(CLUSTER_BSIZE * 2);
	while(length != 0){
		fat_read_block(name, offset_to_cluster(offset), scratch);
		uint index = offset_in_cluster(offset);
		if(CLUSTER_BSIZE - index < length){
			memcpy(buffer, &scratch[index], CLUSTER_BSIZE - index);
			buffer = (uchar*) ((uint) buffer + (CLUSTER_BSIZE - index));
			length -= CLUSTER_BSIZE - index;
		}

		else{
			memcpy(buffer, &scratch[index], length);
			break;
		}
	}
	
	kfree(scratch);
	return 0;
}

uint fat_write(char* name, uint offset, uint length, char* buffer){
	char* fname = fat_name_conv(name);
	fat_dir_t* entry = fat_dir_search(fname);

	if(entry == NULL)
		return (uint) -1;
	
	update_size(entry, offset, length);
	
	if(offset_to_cluster(offset) > fat_cluster_size(entry)){
		uint extension = offset_to_cluster(offset);
		while(extension < offset_to_cluster(offset + length)){
			fat_write_block(name, extension, buffer);
			extension += CLUSTER_BSIZE;
		}
	}

	uchar* scratch = (uchar*) kmalloc(CLUSTER_BSIZE * 2);
	while(length != 0){
		fat_read_block(name, offset_to_cluster(offset), scratch);
		uint index = offset_in_cluster(offset);
		if(CLUSTER_BSIZE - index < length){
			memcpy(&scratch[index], buffer, CLUSTER_BSIZE - index);
			buffer = (uchar*) ((uint) buffer + (CLUSTER_BSIZE - index));
			length -= CLUSTER_BSIZE - index;
		}

		else{
			memcpy(&scratch[index], buffer, length);
			length = 0;
		}
		
		fat_write_block(name, offset_to_cluster(offset), scratch);
	}
	
	kfree(scratch);
	return 0;
}

void fat_sync(){
	NOTIFY("Syncing FAT root directory")
		ata_write_blocks((ushort*) fat_dir, fat_header.number_reserved + FAT_TOTAL_SIZE, (fat_header.dir_size * 32) / 512);
	
	NOTIFY("Syncing FAT cluster tables")
	int i;
	for(i = 0; i < fat_header.number_fats; i++){
		NOTIFY("working...");
		ata_write_blocks(cluster_list, fat_header.number_reserved + (fat_header.sectors_per_fat * i), fat_header.sectors_per_fat);
	}

	//disp_fat_dir();
}
