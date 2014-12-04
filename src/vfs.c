#include "common.h"
#include "monitor.h"
#include "kheap.h"
#include "vfs.h"

uint read_fs(vfs_node_t* node, uint offset, uint size, uchar* buffer){
	if(node->read != NULL)
		return node->read(node, offset, size, buffer);
	else
		return 0;
}
