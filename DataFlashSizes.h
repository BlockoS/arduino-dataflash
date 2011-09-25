/*
********************************************************************************
 DataFlashSizes.h
 
 Header file containing definitions for the sizes of the flash memories in the
 Atmel DataFlash AT45DB range.
 
 
 Copyright (C) 2011 by Volker Kuhlmann.
 http://volker.top.geek.nz/contact.html
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 
1.0  01 Sep 2011   Created.
********************************************************************************
*/

#ifndef DATAFLASH_SIZES_H_
#define DATAFLASH_SIZES_H_

/*

For each flash memory IC these definitions give the page size, block size,
sector size and total size in bytes, the number of pages, blocks and sectors,
and the number of pages in sector 0A (the remaining pages of the first sector
are in sector 0b).

The standard page size for DataFlash has an additional 8 bytes per 256 bytes.
These can be turned off permanently, in which case the page size reverts to a
binary page size with a power of two. This is indicated in the status register.

Sizes below are for the larger (non-binary) page size. To get the binary page
size, mask out an appropriate number of lower bits, or multiply by 256/264.

*/

// AT45DB011D
#define DF_45DB011_PAGESIZE     264
#define DF_45DB011_BLOCKSIZE    (8 * DF_45DB011_PAGESIZE)
#define DF_45DB011_SECTORSIZE   (16 * DF_45DB011_BLOCKSIZE)
#define DF_45DB011_PAGES        512
#define DF_45DB011_BLOCKS       64
#define DF_45DB011_SECTORS      4
#define DF_45DB011_SIZE         (DF_45DB011_PAGES * DF_45DB011_PAGESIZE)
#define DF_45DB011_S0A_PAGES    8

// AT45DB021D
#define DF_45DB021_PAGESIZE     264
#define DF_45DB021_BLOCKSIZE    (8 * DF_45DB021_PAGESIZE)
#define DF_45DB021_SECTORSIZE   (16 * DF_45DB021_BLOCKSIZE)
#define DF_45DB021_PAGES        1024
#define DF_45DB021_BLOCKS       128
#define DF_45DB021_SECTORS      8
#define DF_45DB021_SIZE         (DF_45DB021_PAGES * DF_45DB021_PAGESIZE)
#define DF_45DB021_S0A_PAGES    8

// AT45DB041D
#define DF_45DB041_PAGESIZE     264
#define DF_45DB041_BLOCKSIZE    (8 * DF_45DB041_PAGESIZE)
#define DF_45DB041_SECTORSIZE   (32 * DF_45DB041_BLOCKSIZE)
#define DF_45DB041_PAGES        2048
#define DF_45DB041_BLOCKS       256
#define DF_45DB041_SECTORS      8
#define DF_45DB041_SIZE         (DF_45DB041_PAGES * DF_45DB041_PAGESIZE)
#define DF_45DB041_S0A_PAGES    8

// AT45DB081D
#define DF_45DB081_PAGESIZE     264
#define DF_45DB081_BLOCKSIZE    (8 * DF_45DB081_PAGESIZE)
#define DF_45DB081_SECTORSIZE   (32 * DF_45DB081_BLOCKSIZE)
#define DF_45DB081_PAGES        4096
#define DF_45DB081_BLOCKS       512
#define DF_45DB081_SECTORS      16
#define DF_45DB081_SIZE         (DF_45DB081_PAGES * DF_45DB081_PAGESIZE)
#define DF_45DB081_S0A_PAGES    8

// AT45DB161D
#define DF_45DB161_PAGESIZE     528
#define DF_45DB161_BLOCKSIZE    (8 * DF_45DB161_PAGESIZE)
#define DF_45DB161_SECTORSIZE   (32 * DF_45DB161_BLOCKSIZE)
#define DF_45DB161_PAGES        4096
#define DF_45DB161_BLOCKS       512
#define DF_45DB161_SECTORS      16
#define DF_45DB161_SIZE         (DF_45DB161_PAGES * DF_45DB161_PAGESIZE)
#define DF_45DB161_S0A_PAGES    8

// AT45DB321D
#define DF_45DB321_PAGESIZE     528
#define DF_45DB321_BLOCKSIZE    (8 * DF_45DB321_PAGESIZE)
#define DF_45DB321_SECTORSIZE   (16 * DF_45DB321_BLOCKSIZE)
#define DF_45DB321_PAGES        8192
#define DF_45DB321_BLOCKS       1024
#define DF_45DB321_SECTORS      64
#define DF_45DB321_SIZE         (DF_45DB321_PAGES * DF_45DB321_PAGESIZE)
#define DF_45DB321_S0A_PAGES    8

// AT45DB642D
#define DF_45DB642_PAGESIZE     1056
#define DF_45DB642_BLOCKSIZE    (8 * DF_45DB642_PAGESIZE)
#define DF_45DB642_SECTORSIZE   (32 * DF_45DB642_BLOCKSIZE)
#define DF_45DB642_PAGES        8192
#define DF_45DB642_BLOCKS       1024
#define DF_45DB642_SECTORS      32
#define DF_45DB642_SIZE         (DF_45DB642_PAGES * DF_45DB642_PAGESIZE)
#define DF_45DB642_S0A_PAGES    8


#endif  // DATAFLASH_SIZES_H_

/******************************************************************************/
