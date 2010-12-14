/*
 * mfs.h
 *
 *  Created on: Dec 7, 2010
 *      Author: pro
 */

#ifndef MFS_H_
#define MFS_H_

#include <stdio.h>
#include <_ansi.h>
#include "platform.h"
#include "xbasic_types.h"
#include "xilmfs.h"

// for sphinx_data.mfs
#define FLASH_BASE_ADDR 0xc4000000
#define MFS_ORIG_ADDR 0x800000
#define NUMBLOCKS 37000
#define MFS_FILE_SIZE 0x12C5AA4

#ifndef _NO_MFS
// replace file operation functions in stdio.h
#define fopen	mfs_file_open_wrapper
#define fclose	mfs_file_close_wrapper
#define fread	mfs_file_read_wrapper
#define fgetc	mfs_file_getc_wrapper
#define fgets	mfs_file_gets_wrapper
#define fwrite	mfs_file_write_wrapper
#define fseek	mfs_file_lseek_wrapper
#define ftell	mfs_file_tell_wrapper
#define feof	mfs_file_eof_wrapper
#define fflush	mfs_file_flush_wrapper
#define rewind mfs_file_rewind_wrapper
#endif

// alternate FILE structure
typedef struct _mfs_file {
	int fd;
} MFS_FILE;

// replace FILE structure
#define FILE MFS_FILE

// wrappers for file operation
FILE* mfs_file_open_wrapper(const char *_name, const char *_type);
int	mfs_file_close_wrapper(FILE *);
size_t mfs_file_read_wrapper(_PTR, size_t _size, size_t _n, FILE *);
int mfs_file_getc_wrapper(FILE *fp);
char *mfs_file_gets_wrapper(char *buf, int size, FILE *fp);
size_t mfs_file_write_wrapper(const _PTR buf, size_t _size, size_t _n, FILE *fp);
long mfs_file_lseek_wrapper(FILE *fp, long offset, int whence);
long mfs_file_tell_wrapper(FILE *fp);
int mfs_file_eof_wrapper(FILE *fp);
int mfs_file_flush_wrapper(FILE *fp);
void mfs_file_rewind_wrapper(FILE *fp);

int xps_mfs_init();

#endif /* MFS_H_ */
