/*
Arduino Library to access Atmel DataFlash flash memory ICs with SPI interface.

History:

Version 1.x, 2010-2011

Released as at45db161d by ...
[Add / change.]


Version 2.0, 30 Aug 2011.

Based on the library by BlockoS, dalek branch, of 25 Aug 2011.
http://github.com/BlockoS/arduino-dataflash/tarball/dalek
Substantially modified and improved by Volker Kuhlmann.

 - Allow a quick .begin() / .end() to switch the SPI interface between
   multiple SPI devices.
 - Efficiency improvements.
 - Re-arrange the mechanism to wait for the chip to become ready such that
   waiting only happens when necessary. This allows interleaved writing - fill
   up one buffer with new data while the other buffer is programmed to the 
   flash memory array. The downside is the the user now has to wait sometimes
   too, depending on the state of the flash chip and the user program.
 - Several improvements resulted in incompatible changes to the function API.
   This shouldn't matter because the DataFlash library is in the process of
   evolving as an improvement of the at45db161d library and handles all the
   AT45DBxxxD flash ICs instead of just the AT45DB161D.


Copyright (C) 2010-2011 by Vincent Cruz.
cruz.vincent@gmail.com

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
*/

/**
 * @file DataFlashCommands.h
 * @brief AT45DBxxxD commands opcodes
 **/
#ifndef DATAFLASH_COMMANDS_H_
#define DATAFLASH_COMMANDS_H_

/**
 * @defgroup dataflash_commands DataFlash commands opcodes
 * @{
 **/

/**
 * @defgroup Read_commands Read commands
 * @{
 **/
/** Main Memory Page Read */
#define DATAFLASH_PAGE_READ 0xD2
/** Continuous Array Read (Low Frequency) **/
#define DATAFLASH_CONTINUOUS_READ_LOW_FREQ 0x03
/** Continuous Array Read (High Frequency) **/ 
#define DATAFLASH_CONTINUOUS_READ_HIGH_FREQ 0x0B
/** Buffer 1 Read (Low Frequency) **/
#define DATAFLASH_BUFFER_1_READ_LOW_FREQ 0xD1
/** Buffer 2 Read (Low Frequency) **/
#define DATAFLASH_BUFFER_2_READ_LOW_FREQ 0xD3
/** Buffer 1 Read **/
#define DATAFLASH_BUFFER_1_READ 0xD4
/** Buffer 2 Read **/
#define DATAFLASH_BUFFER_2_READ 0xD6
/**
 * @}
 **/

/**
 * @defgroup Program_Erase_commands Program and Erase commands
 * @{
 **/
/** Buffer 1 Write **/
#define DATAFLASH_BUFFER_1_WRITE 0x84
/** Buffer 2 Write **/
#define DATAFLASH_BUFFER_2_WRITE 0x87
/** Buffer 1 to Main Memory Page Program with Built-in Erase **/
#define DATAFLASH_BUFFER_1_TO_PAGE_WITH_ERASE 0x83
/** Buffer 2 to Main Memory Page Program with Built-in Erase **/
#define DATAFLASH_BUFFER_2_TO_PAGE_WITH_ERASE 0x86
/** Buffer 1 to Main Memory Page Program without Built-in Erase **/
#define DATAFLASH_BUFFER_1_TO_PAGE_WITHOUT_ERASE 0x88
/** Buffer 2 to Main Memory Page Program without Built-in Erase **/
#define DATAFLASH_BUFFER_2_TO_PAGE_WITHOUT_ERASE 0x89
/** Page Erase **/
#define DATAFLASH_PAGE_ERASE 0x81
/** Block Erase **/
#define DATAFLASH_BLOCK_ERASE 0x50
/** Sector Erase **/
#define DATAFLASH_SECTOR_ERASE 0x7C
/** Chip Erase Sequence **/
#define DATAFLASH_CHIP_ERASE_0 0xC7
#define DATAFLASH_CHIP_ERASE_1 0x94
#define DATAFLASH_CHIP_ERASE_2 0x80
#define DATAFLASH_CHIP_ERASE_3 0x9A
/** Main Memory Page Program Through Buffer 1 **/
#define DATAFLASH_PAGE_THROUGH_BUFFER_1 0x82
/** Main Memory Page Program Through Buffer 2 **/
#define DATAFLASH_PAGE_THROUGH_BUFFER_2 0x85
/**
 * @}
 **/

/**
 * @defgroup ProtectionSecurity_Commands Protection and Security Commands
 * @warning UNIMPLEMENTED
 * @{
 **/
#ifdef DATAFLASH_EXPERT_MODE
/* Use the following commands at your own risk ! */
/** Enable Sector Protection **/
#define DATAFLASH_ENABLE_SECTOR_PROTECTION_0 0x3D
#define DATAFLASH_ENABLE_SECTOR_PROTECTION_1 0x2A
#define DATAFLASH_ENABLE_SECTOR_PROTECTION_2 0x7F
#define DATAFLASH_ENABLE_SECTOR_PROTECTION_3 0xA9
/** Disable Sector Protection **/
#define DATAFLASH_DISABLE_SECTOR_PROTECTION_0 0x3D
#define DATAFLASH_DISABLE_SECTOR_PROTECTION_1 0x2A
#define DATAFLASH_DISABLE_SECTOR_PROTECTION_2 0x7F
#define DATAFLASH_DISABLE_SECTOR_PROTECTION_3 0x9A
/** Erase Sector Protection Register **/
#define DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_0 0x3D
#define DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_0 0x2A
#define DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_0 0x7F
#define DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_0 0xCF
/** Program Sector Protection Register **/
#define DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_0 0x3D
#define DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_1 0x2A
#define DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_2 0x7F
#define DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_3 0xFC
/** Sector Lockdown **/
#define DATAFLASH_SECTOR_LOCKDOWN_0 0X3D
#define DATAFLASH_SECTOR_LOCKDOWN_1 0x2A
#define DATAFLASH_SECTOR_LOCKDOWN_2 0x7F
#define DATAFLASH_SECTOR_LOCKDOWN_3 0x30
/** Program Security Register **/
#define DATAFLASH_PROGRAM_SECURITY_REGISTER_0 0x9B
#define DATAFLASH_PROGRAM_SECURITY_REGISTER_1 0x00
#define DATAFLASH_PROGRAM_SECURITY_REGISTER_2 0x00
#define DATAFLASH_PROGRAM_SECURITY_REGISTER_3 0x00
#endif /* DATAFLASH_EXPERT_MODE */

/** Read Sector Protection Register **/
#define DATAFLASH_READ_SECTOR_PROTECTION_REGISTER 0x32
/** Read Sector Lockdown Register **/
#define DATAFLASH_READ_SECTOR_LOCKDOWN_REGISTER 35H
/** Read Security Register **/
#define DATAFLASH_READ_SECURITY_REGISTER 0x77
/**
 * @}
 **/

/**
 * @defgroup Additional_commands Additional Commands
 * @{
 **/
/** Main Memory Page to Buffer 1 Transfer **/
#define DATAFLASH_TRANSFER_PAGE_TO_BUFFER_1 0x53
/** Main Memory Page to Buffer 2 Transfer **/
#define DATAFLASH_TRANSFER_PAGE_TO_BUFFER_2 0x55
/** Main Memory Page to Buffer 1 Compare **/
#define DATAFLASH_COMPARE_PAGE_TO_BUFFER_1 0x60
/** Main Memory Page to Buffer 2 Compare **/
#define DATAFLASH_COMPARE_PAGE_TO_BUFFER_2 0x61
/** Auto Page Rewrite through Buffer 1 **/
#define DATAFLASH_AUTO_PAGE_REWRITE_THROUGH_BUFFER_1 0x58
/** Auto Page Rewrite through Buffer 2 **/
#define DATAFLASH_AUTO_PAGE_REWRITE_THROUGH_BUFFER_2 0x59
/** Deep Power-down **/
#define DATAFLASH_DEEP_POWER_DOWN 0xB9
/** Resume from Deep Power-down **/
#define DATAFLASH_RESUME_FROM_DEEP_POWER_DOWN 0xAB
/** Status Register Read **/
#define DATAFLASH_STATUS_REGISTER_READ 0xD7
/** Manufacturer and Device ID Read **/
#define DATAFLASH_READ_MANUFACTURER_AND_DEVICE_ID 0x9F
/**
 * @}
 **/

/**
 * @defgroup Legacy_commands Legacy Commands
 * @{
 **/
/** Buffer 1 Read **/
#define DATAFLASH_BUFFER_1_READ_LEGACY 0X54
/** Buffer 2 Read **/
#define DATAFLASH_BUFFER_2_READ_LEGACY 0x56
/** Main Memory Page Read **/
#define DATAFLASH_PAGE_READ_LEGACY 0x52
/** Continuous Array Read **/
#define DATAFLASH_CONTINUOUS_READ_LEGACY 0x68
/** Status Register Read **/
#define DATAFLASH_STATUS_REGISTER_READ_LEGACY 0x57
/**
 * @}
 **/

/**
 * @}
 **/

#endif /* DATAFLASH_COMMANDS_H_ */
