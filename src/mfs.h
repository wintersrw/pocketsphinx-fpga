/*
 * mfs.h
 *
 *  Created on: Dec 7, 2010
 *      Author: pro
 */

#ifndef MFS_H_
#define MFS_H_

#include <stdio.h>
#include "platform.h"
#include "xbasic_types.h"
#include "xilmfs.h"

// for sphinx_data.mfs
#define FLASH_BASE_ADDR 0xc4000000
#define MFS_ORIG_ADDR 0x800000
#define NUMBLOCKS 37000
#define MFS_FILE_SIZE 0x12C5AA4

int xps_mfs_init();

#endif /* MFS_H_ */
