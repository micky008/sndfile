/*
	mpglib: test program for libmpg123, in the style of the legacy mpglib test program

	copyright 2007 by the mpg123 project - free software under the terms of the LGPL 2.1
	see COPYING and AUTHORS files in distribution or http://mpg123.org
	initially written by Thomas Orgis
*/

#include <mpg123.h>
#include <stdio.h>
#include <string.h>

void demux(short in[], short left[], short right[], int len)
{
	for (int i = 0; i < len / 2; i + 2)
	{
		left[i] = in[i];
		right[i + 1] = in[i + 1];
	}
}

int main(int argc, char **argv)
{
	size_t size;
	ssize_t len;
	int ret = 0;
	size_t in = 0, outc = 0;
	mpg123_handle *m;

	mpg123_init();
	m = mpg123_new(NULL, &ret);
	if (m == NULL)
	{
		fprintf(stderr, "Unable to create mpg123 handle: %s\n", mpg123_plain_strerror(ret));
		return -1;
	}
	mpg123_open(m, "c:/ffmpeg/cbt.mp3");
	if (m == NULL)
		return -1;

	long rate;
	int channels, enc;
	mpg123_getformat(m, &rate, &channels, &enc);
	mpg123_scan(m);
	off_t nbSample = mpg123_length(m);
	short *buf = malloc(nbSample * sizeof(short));
	size_t s = 1;
	int h = 0;
	while (ret != MPG123_DONE)
	{
		ret = mpg123_read(m, buf, nbSample, &s);
		printf("end = %d\n", ret);

		if (ret == MPG123_DONE)
		{
			continue;
		}
		if (ret != MPG123_OK)
		{
			puts(mpg123_plain_strerror(ret));
			break;
		}
		printf("s = %d\n", s);
		puts("\n");
		h++;
	}
	free(buf);
	fprintf(stderr, "Some error occured (non-fatal?): %s\n", mpg123_strerror(m));
	/* Done decoding, now just clean up and leave. */
	mpg123_delete(m);
	mpg123_exit();
	return 0;
}
