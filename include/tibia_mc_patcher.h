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

#ifndef _TIBIA_MC_PATCHER_H
#define _TIBIA_MC_PATCHER_H

/* Note: All VMAs and strings were took from GDB while reverse engineering Tibia
 * client.
 */
#define TIBIA_X_ATOM_NAME_STR         "TIBIARUNNING"
#define TIBIA_X_ATOM_NAME_VMA         0x83c4e01ul
#define TIBIA_X_PATCH_AREA_VMA        0x8332169ul

/*
 * Tibia's section headers:
 *
 * [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
 * ...
 * ...
 * [13] .text             PROGBITS        0804d5d0 0055d0 35e4ec 00  AX  0   0 16
 * ...
 * [15] .rodata           PROGBITS        083abae0 363ae0 021dbc 00   A  0   0 32
 * ...
 * ...
 */

#define TIBIA_TEXT_SECTION_VMA    0x804d5d0ul
#define TIBIA_TEXT_SECTION_OFFS   0x0055d0ul
#define TIBIA_TEXT_SECTION_SIZE   0x35e4ecul

#define TIBIA_RODATA_SECTION_VMA  0x83abae0ul
#define TIBIA_RODATA_SECTION_OFFS 0x363ae0ul
#define TIBIA_RODATA_SECTION_SIZE 0x021dbcul

#define TIBIA_TEXT_VMA_TO_FILE_OFFS(vma, offs)				\
	do {								\
		(offs) = (vma) - TIBIA_TEXT_SECTION_VMA +		\
			TIBIA_TEXT_SECTION_OFFS;			\
		if ((offs) >						\
		    (TIBIA_TEXT_SECTION_OFFS + TIBIA_TEXT_SECTION_SIZE)) { \
			fprintf(stderr,					\
				".text section - VMA to file offset "	\
				"failed\n");				\
			(offs) = 0;					\
		}							\
	} while (0)

#define TIBIA_RODATA_VMA_TO_FILE_OFFS(vma, offs)			\
	do {								\
		(offs) = (vma) - TIBIA_RODATA_SECTION_VMA +		\
			TIBIA_RODATA_SECTION_OFFS;			\
		if ((offs) >						\
		    (TIBIA_RODATA_SECTION_OFFS + TIBIA_RODATA_SECTION_SIZE)) { \
			fprintf(stderr,					\
				".text section - VMA to file offset "	\
				"failed\n");				\
			(offs) = 0;					\
		}							\
	} while (0)

extern int tibia_mc_patch_do_apply(const char *path);

#endif /* _TIBIA_MC_PATCHER_H */
