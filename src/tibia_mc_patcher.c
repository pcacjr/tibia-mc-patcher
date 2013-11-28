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
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "tibia_mc_patcher.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define MC_PATCH_CODE_SIZE 8

/*
 *  mc_patch_code contains NOP's to be patched into these two instructions
 *  below:
 *
 *  0x8332169:	test   eax,eax
 *  0x833216b:	jne    0x83323a6
 */
static const u8 mc_patch_code[MC_PATCH_CODE_SIZE] = {
	0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
};

/* A few sanity checks to make sure it will work fine. */
static int do_sanity_checks(u8 *map)
{
	u32 offs;
	u8 *p;
	int ok = -1;

	/* Check if "TIBIARUNNING" string is in the vma we expect */
	TIBIA_RODATA_VMA_TO_FILE_OFFS(TIBIA_X_ATOM_NAME_VMA, offs);
	if (offs) {
		printf("vma 0x%lx to file offset 0x%lx\n",
		       TIBIA_X_ATOM_NAME_VMA, (unsigned long)offs);

		p = map + offs;
		if (strcmp((const char *)p, TIBIA_X_ATOM_NAME_STR))
			ok = -1; /* No, it isn't. :-( */
		else
			ok = 0; /* Good. :-) */
	}

	/* TODO: add more checks */

	return ok;
}

/* Apply MC's hack into Tibia client */
static int apply_mc_patch(u8 *map)
{
	u32 offs;
	int ok = -1;

	/* Add NOP's at TIBIA_X_PATCH_AREA_VMA to disable check which avoids
	 * mulitple X clients for Tibia to be running.
	 */
	TIBIA_TEXT_VMA_TO_FILE_OFFS(TIBIA_X_PATCH_AREA_VMA, offs);
	if (offs) {
		memcpy(map + offs, &mc_patch_code, MC_PATCH_CODE_SIZE);
		ok = 0;
	}

	return ok;
}

int tibia_mc_patch_do_apply(const char *path)
{
	int fd;
	int retval;
	struct stat sb;
	u8 *map;

	fd = open(path, O_RDWR | O_NOFOLLOW | O_NONBLOCK);
	if (fd < 0) {
		perror("open()");
		goto open_failed;
	}

	retval = fstat(fd, &sb);
	if (retval < 0) {
		perror("fstat()");
		goto stat_failed;
	}

	map = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
		   MAP_FILE | MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		perror("mmap()");
		goto mmap_failed;
	}

	retval = do_sanity_checks(map);
	if (retval < 0) {
		fprintf(stderr, "Sanity checks failed\n");
		goto sanity_checks_failed;
	}

	printf("Cool - sanity check(s) passed OK!\n");

	retval = apply_mc_patch(map);
	if (retval < 0) {
		fprintf(stderr, "Failed to apply MC patch. Sorry :-(\n");
		goto patch_failed;
	}

	printf("Patch has been applied successfully!\n");

	munmap(map, sb.st_size);
	close(fd);

	return 0;

patch_failed:

sanity_checks_failed:
	munmap(map, sb.st_size);

mmap_failed:

stat_failed:
	close(fd);

open_failed:
	return -1;
}
