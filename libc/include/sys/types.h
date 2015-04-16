#ifndef __TYPE_H
#define __TYPE_H

typedef unsigned int 	u32int;
typedef int		s32int;
typedef unsigned short 	u16int;
typedef short 		s16int;
typedef unsigned char	u8int;
typedef char		s8int;
typedef int		size_t;
typedef unsigned int	uint;
typedef unsigned char	uchar;
typedef unsigned short	ushort;

#define NULL 0
#define FALSE 0
#define TRUE (!FALSE)

#define dev_t		uint
#define ino_t		uint
#define mode_t		uint
#define nlink_t		uint
#define uid_t		uint
#define gid_t		uint
#define off_t		uint
#define time_t		uint
#define blksize_t	uint
#define blkcnt_t	uint

#endif
