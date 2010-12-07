/*
 * mfs.c
 *
 *  Created on: Dec 7, 2010
 *      Author: pro
 */

#include "mfs.h"

/**
 * Initialize the memory file system used for sphinx data.
 */
int xps_mfs_init()
{
	char *fs = (char *)(FLASH_BASE_ADDR+MFS_ORIG_ADDR); /* base address of MFS */
	int numbytes;
	numbytes = NUMBLOCKS *sizeof(struct mfs_file_block);
	mfs_init_genimage(numbytes, fs, MFSINIT_ROM_IMAGE);

	return 0;
}
