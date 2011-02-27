/*
 * ps_hello.c
 *
 *  Created on: Dec 7, 2010
 *      Author: Bill Chen <pro711@gmail.com>
 */

#include <stdio.h>
#include "platform.h"
#include <pocketsphinx.h>
#include "mfs.h"

int decode()
{
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE *fh;
	char const *hyp, *uttid;
	int16 buf[512];
	int rv;
	int32 score;

	config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", "/model/hmm/en_US/hub4wsj_sc_8k",
			     "-lm", "/model/lm/en/turtle.DMP",
			     "-dict", "/model/lm/en/turtle.dic",
			     "-mmap", "0",
                 "-debug", "1",
                 "-verbose", "1",
			     "-bestpath", "0",
			     "-input_endian", "little",
			     "-samprate", "16000",
//			     "-logspec", "1",
			     NULL);
	if (config == NULL)
		return 1;
	ps = ps_init(config);
	if (ps == NULL)
		return 1;

	fh = fopen("/audio/goforward.raw", "rb");
	if (fh == NULL) {
		perror("Failed to open goforward.raw");
		return 1;
	}
//	long pos;
//	fseek(fh, -1, SEEK_END);
//	pos = ftell(fh);
//	fseek(fh, 0, SEEK_END);
//	pos = ftell(fh);

	rv = ps_decode_raw(ps, fh, "goforward", -1);
	if (rv < 0)
		return 1;
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		return 1;
	printf("Recognized: %s\n", hyp);

        fseek(fh, 0, SEEK_SET);
        rv = ps_start_utt(ps, "goforward");
	if (rv < 0)
		return 1;
        while (!feof(fh)) {
            size_t nsamp;
            nsamp = fread(buf, 2, 512, fh);
            rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
        }
        rv = ps_end_utt(ps);
	if (rv < 0)
		return 1;
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		return 1;
	printf("Recognized: %s\n", hyp);

	fclose(fh);
        ps_free(ps);
    return 0;
}

static int
file_exists(const char *path)
{
    FILE *tmp;

    tmp = fopen(path, "rb");
    if (tmp) fclose(tmp);
    return (tmp != NULL);
}

int main_ps_hello()
{
	char *p;
	int i;

    init_platform();

    print("Running ps_hello...\n\r");

    xps_mfs_init();
//    printf("%d\n",sizeof(long long));
//    if (file_exists("/model/hmm/en_US/hub4wsj_sc_8k/feat.params"))
//    	puts("file exists");
//    else
//    	puts("file does not exist");
//	FILE *fp = fopen("/audio/goforward.raw", "rb");
//	if (!fp) {
//		perror("Failed to open goforward.raw");
//		return 1;
//	}
//	printf("%X\n",ftell(fp));
//	fseek(fp,5,SEEK_SET);
//	printf("%X\n",ftell(fp));
//	fseek(fp,1,SEEK_CUR);
//	printf("%X\n",ftell(fp));
//	fseek(fp,0,SEEK_END);
//	printf("%X\n",ftell(fp));
//	fseek(fp,7,SEEK_SET);
//	printf("%X\n",ftell(fp));
//    for (i=0;i<1000;i++) {
//		p = ckd_calloc(2713,4);
//
//		if (!p) {
//			puts("calloc failed");
//			return 0;
//		} else {
//			printf("%4d address: %X\n", i, p);
////			free(p);
//		}
//    }
    malloc(0x200000);
    // start decode
    decode();
//    FILE* fp;
//    char buf[256];
////    buf[16]=0;
////    puts(buf);
//    fp=fopen("/readme.txt","rb");
//    fread(buf, 1, 16, fp);
////    int fd;
////    fd=mfs_file_open("/readme.txt",MFS_MODE_READ);
////    mfs_file_read(fd,buf,16);
//    buf[16]=0;
//    puts(buf);


    cleanup_platform();

    return 0;
}

