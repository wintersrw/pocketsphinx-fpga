/*
 * mfs.c
 * Memory File System Helper Library
 *
 *  Created on: Dec 7, 2010
 *      Author: pro
 */

#include "mfs.h"
#include <stdlib.h>

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

FILE* mfs_file_open_wrapper(const char *_name, const char *_type)
{
	int fd;

	// FIXME: MFS_MODE_READ is used, error on r/w file system
	if (_type[0] == 'w') {
		perror("Trying to open a file for write on read-only file system!");
		perror(_name);
		return NULL;
	}

	fd = mfs_file_open(_name,MFS_MODE_READ);
	if (fd == -1)
		return NULL;

	FILE *fp = (FILE*)malloc(sizeof(FILE));
	if (!fp)
		return NULL;

	fp->fd = fd;
	return fp;
}

int	mfs_file_close_wrapper(FILE * fp)
{
	int retval;

	if (!fp)
		return EOF;
	retval =  mfs_file_close(fp->fd);
	if (fp)
		free(fp);
	return retval == 1 ? 0 : EOF;
}

size_t mfs_file_read_wrapper(_PTR buf, size_t _size, size_t _n, FILE *fp)
{
	int len = _size*_n;
	int n_bytes;
	n_bytes = mfs_file_read(fp->fd, buf, _size*_n);

	if (n_bytes == len)
		return _n;
	else
		return n_bytes/_size;
}

int mfs_file_getc_wrapper(FILE *fp)
{
	int count;
	char c;

	count = mfs_file_read(fp->fd, &c, 1);

	return count == 0 ? EOF : (int)c;
}

char *mfs_file_gets_wrapper(char *buf, int size, FILE *fp)
{
	int count, len;
	char c;

	for (count=0;count<size-1;count++) {
		len = mfs_file_read(fp->fd, &c, 1);
		if (!len) {
			if (!count)
				return NULL;
			else {
				buf[count] = '\0';
				return buf;
			}
		} else {
			buf[count] = c;
			if (c == '\n') {
				buf[count+1] = '\0';
				break;
			}
		}
	}

	return buf;

}

size_t mfs_file_write_wrapper(const _PTR buf, size_t _size, size_t _n, FILE *fp)
{
	return 0;
}

long mfs_file_lseek_wrapper(FILE *fp, long offset, int whence)
{
	int retval;

	if (whence == SEEK_CUR)
		whence = MFS_SEEK_CUR;
	else if (whence == SEEK_SET)
		whence = MFS_SEEK_SET;
	else if (whence == SEEK_END)
		whence = MFS_SEEK_END;

	// workaround for a bug in mfs_filesys.c
	if (offset == 0 && whence == MFS_SEEK_END) {
		mfs_file_lseek(fp->fd, -1, whence);
		retval = mfs_file_lseek(fp->fd, 1, MFS_SEEK_CUR);
		if (retval == -1)
			perror("Error seeking to file end!");
	} else {
		retval = mfs_file_lseek(fp->fd, offset, whence);
	}

	if (retval > 0)
		retval = 0;

	return retval;
}

long mfs_file_tell_wrapper(FILE *fp)
{
	return mfs_file_lseek(fp->fd, 0, MFS_SEEK_CUR);
}

int mfs_file_eof_wrapper(FILE *fp)
{
	long cur_pos, end_pos;

	cur_pos = mfs_file_tell_wrapper(fp);
	mfs_file_lseek_wrapper(fp, 0, MFS_SEEK_END);
	end_pos = mfs_file_tell_wrapper(fp);
	mfs_file_lseek_wrapper(fp, cur_pos, MFS_SEEK_SET);

	return cur_pos == end_pos ? 1 : 0;
}

int mfs_file_flush_wrapper(FILE *fp)
{
//	puts("fflush called\n");
	return 0;
}

void mfs_file_rewind_wrapper(FILE *fp)
{
	mfs_file_lseek_wrapper(fp,0,MFS_SEEK_SET);
}
