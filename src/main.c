/*
 * Copyright (c) 2013 Paulo Alcantara <pcacjr@zytor.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "tibia_mc_patcher.h"

static struct option long_options[] = {
	{ "path",         required_argument, 0, 'p' },
	{ "help",         no_argument,       0, 'h' },
	{ "version",      no_argument,       0, 'v' },
	{ 0, 0, 0, 0 },
};

int main(int argc, char **argv)
{
	int option_index = 0;
	int c;
	const char *path;

	for (;;) {
		c = getopt_long(argc, argv, "p:hv", long_options,
				&option_index);
		if (c < 0)
			break;

		switch (c) {
		case 'p':
			path = optarg;
			break;
		case 'h':
			printf("TODO: Print program help\n");
			break;
		case 'v':
			printf("TODO: Print program version\n");
			break;
		default:
			printf("Weird - returned character code 0%o ??\n", c);
			goto error;
		}
	}

	if (path && *path)
		tibia_mc_patch_do_apply(path);

	return 0;

error:
	exit(EXIT_FAILURE);
}
