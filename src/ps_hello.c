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

int main()
{
    init_platform();

    print("Running ps_hello...\n\r");

    xps_mfs_init();

    // start decode
    decode();

    cleanup_platform();

    return 0;
}

