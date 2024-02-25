/**************************************************************************//**
 * @file DataFlash.h
 * @brief AT45DBxxxD Atmel Dataflash library for Arduino.
 *
 * @par Copyright: 
 * - Copyright (C) 2010-2023 by Vincent Cruz.
 * - Copyright (C) 2011 by Volker Kuhlmann. @n
 * All rights reserved.
 *
 * @authors
 * - Vincent Cruz @n
 *   mooz@blockos.org
 * - Volker Kuhlmann @n
 *   http://volker.top.geek.nz/contact.html
 *
 * @par Description:
 * Please refer to @ref DataFlash.cpp for more informations.
 *
 * @par History:
 * - Version 1.x, 2010-2011.
 * - Version 2.0, 30 Aug 2011.
 * - Version 2.2, 29 Dec 2011.
 *
 * @par Licence: GPLv3
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. @n
 * @n
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. @n
 * @n
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef DATAFLASH_H
#define DATAFLASH_H

#include <stdint.h>
#include <stdbool.h>

#include "DataFlashSizes.h"

/**
 * @mainpage Atmel Dataflash library for Arduino.
 *
 * http://www.atmel.com/products/memories/sflash/dataflash.aspx
 **/

/**
 * @defgroup AT45DBxxxD Atmel Dataflash library for Arduino.
 * @{
 **/

/**
 * @defgroup AT45_CHIP_ERASE_ENABLED Chip erase command prevention.
 * @note Will be removed once chip erase is re-implemented.
 * Datasheets are missing errata, but see AT45DB321D. Basically the
 * silicon is buggy and Atmel suggests to use block erase instead,
 * giving rise to the suspicion that sector erase doesn't work either.
 * @{
 **/
#ifdef AT45_CHIP_ERASE_ENABLED
#undef AT45_CHIP_ERASE_ENABLED
#endif
/**
 * @}
 **/

/**
 * @defgroup AT45_USE_SPI_SPEED_CONTROL SPI transfer speed control.
 * @warning This feature is experimental. Use it at your own risk!
 * %Dataflash supports low and high speed transfers. However the low
 * speed transfers are more than 3x the speed of what an Arduino
 * with ATmega 328P or 1280 can provide, so this makes no sense at
 * all with that hardware. 
 * Leaving in, in case it's useful for other hardware. Tested code.
 * Dropping this saves 86 bytes.
 * @{
 **/
#ifdef AT45_USE_SPI_SPEED_CONTROL
#undef AT45_USE_SPI_SPEED_CONTROL
#endif
/**
 * @}
 **/

/**
 * @defgroup PINOUT Default pin connections.
 * Default pin values for Chip Select (CS), Reset (RS) and
 * Write Protec (WP). 
 * Reset and Write Protect pins are not used by default.
 * @{
 **/
/** Chip select (CS) **/
#define AT45_SS_PIN     10
/** Reset (Reset) **/
#define AT45_RESET_PIN  -1
/** Write protect (WP) **/
#define AT45_WP_PIN     -1
/**
 * @}
 **/

/**
 * @defgroup STATUS_REGISTER_FORMAT Status register format.
 * The status register can be used to determine device state
 * (ready/busy) or to retrieve the result of an operation.
 * @{
 **/
/**
 * Ready/busy status is indicated using bit 7 of the status register.
 * If bit 7 is a 1, then the device is not busy and is ready to accept
 * the next command. If bit 7 is a 0, then the device is in a busy
 * state.
 **/
#define AT45_READY 0x80
/**
 * Result of the most recent Memory Page to Buffer Compare operation.
 * If this bit is equal to 0, then the data in the main memory page
 * matches the data in the buffer. If it's 1 then at least 1 byte in
 * the main memory page does not match the data in the buffer.
 **/
#define AT45_COMPARE 0x40
/**
 * Bit 1 in the Status Register is used to provide information to the
 * user whether or not the sector protection has been enabled or
 * disabled, either by software-controlled method or
 * hardware-controlled method. 1 means that the sector protection has
 * been enabled and 0 that it has been disabled.
 **/
#define AT45_PROTECT 0x02
/**
 * Bit 0 indicates whether the page size of the main memory array is
 * configured for "power of 2" binary page size (512 bytes) (bit=1) or
 * standard %Dataflash page size (528 bytes) (bit=0).
 **/
#define AT45_PAGESIZE_PWR2 0x01
/**
 * Bits 5, 4, 3 and 2 indicates the device density. The decimal value
 * of these four binary bits does not equate to the device density; the
 * four bits represent a combinational code relating to differing
 * densities of %Dataflash devices. The device density is not the same
 * as the density code indicated in the JEDEC device ID information.
 * The device density is provided only for backward compatibility.
 **/
#define AT45_SIZE_CODE 0x2C
/**
 * @}
 **/

 /**
 * @defgroup SPECIFIC_SECTORS Special sectors ID.
 * The following list gives the number of pages per sector (P) for the AT45 family:
 *   - AT45DB011D 128 
 *   - AT45DB021D 128
 *   - AT45DB041D 256
 *   - AT45DB081D 256
 *   - AT45DB161D 256
 *   - AT45DB321D 128
 *   - AT45DB642D 256
 *
 * On every %Dataflash, the first 8 pages belongs to the sectod 0a. It's followed 
 * by sector 0b which holds only (P-8) pages (248 on an AT45DB161D). Then comes N-1 
 * (N is the number of sectors) sectors of size P numbered from 1 to N-1 (included).
 * @see chapter titled "Memory Array" in the corresponding %Dataflash datasheet.
 * @{
 **/
/**
 * Sector 0a id.
 **/
#define AT45_SECTOR_0A -1
/**
 * Sector 0b id.
 **/
#define AT45_SECTOR_0B 0
 /**
  * @}
  **/
 
/**
 * @brief Erase mode.
 * Whether pages are erased automatically before being written, or 
 * whether this is expected to be done explicitly first.
 **/
typedef enum {
    ERASE_AUTO,   /**< Pages are erased automatically. */
    ERASE_MANUAL, /**< Pages are erased by the user first. */
} DataFlashEraseMode;

/** 
 * @brief IO speed.
 * The max SPI SCK frequency an ATmega 328P or 1280 can generate is
 * 10MHz. The limit for low-speed SCK for AT45DBxxxD %Dataflash is 33MHz
 * (66MHz for high-speed). Supporting high-speed for Arduino is a waste of
 * time...
 **/
typedef enum {
    SPEED_LOW,  /**< Low speed transfers up to 33MHz **/
    SPEED_HIGH, /**< High speed transfers up to 66MHz **/
} DataFlashIOSpeed;

/**
 * AT45DBxxxD Atmel/Adesto/Digital/Renesas... %Dataflash device.
 **/
typedef struct {
    /**
     * @brief ID structure.
     * This structure contains information about the %Dataflash chip being used.
     **/
    struct {
        uint8_t manufacturer;           /**< Manufacturer id */
        uint8_t device[2];              /**< Device id */
        uint8_t extended_info_length;   /**< Extended device information string length */
    } id;

    uint8_t device_index;               /**< Device index. (0: at45db011d, 1: at45db041d, ...) **/
        
    struct {
        uint8_t buffer;                 /**< Size of the buffer address bits. **/
        uint8_t page;                   /**< Size of the page address bits. **/
        uint8_t sector;                 /**< Size of the sector address bits. **/
    } bit_size;

    DataFlashEraseMode erase;           /**< Erase mode - auto or manual. **/

#ifdef AT45_USE_SPI_SPEED_CONTROL
    DataFlashIOSpeed  speed;            /**< SPI transfer speed. **/
#endif

    /** Associated user data (platform specific spi handle, etc...) **/
    uintptr_t user_data;
} DataFlash;


/**
 * Sleep for t microseconds.
 * @param t Number of microseconds to sleep.
 */
extern void dataflash_usleep(uint64_t t);
/**
 * Enable (select) %Dataflash.
 * @param [in out] df DataFlash handle
 */
extern void dataflash_enable(DataFlash *df);
/**
 * Disable (select) %Dataflash.
 * @param [in out] df DataFlash handle
 */
extern void dataflash_disable(DataFlash *df);
/**
 * Transmit and receive bytes (blocking mode).
 * The same same buffer is used as source and destination.
 * @param [in out] df DataFlash handle
 * @param [in] buffer Pointer to the array containing the data send and where the data read from SPI will be stored. 
 * @param [in] size Number of bytes to be sent and received.
 * @param [in] timeout Timeout duration.
 * @return A negative value if an error occured.
 */
extern int dataflash_transfer(DataFlash *df, uint8_t *buffer, uint32_t size, uint32_t timeout);
/**
 * Toggle reset pin.
 * @param [in out] df DataFlash handle
 * @param [in] state Reset pin state 
 * @return true If the reset pin was toggled
 * @return false Otherwise
 */
extern bool dataflash_toggle_reset(DataFlash *df, bool state);
/**
 * 
 * @param [in out] df DataFlash handle
 * @param state 
 * @return true 
 * @return false 
 */
extern bool dataflash_write_protect(DataFlash *df, bool state);

/**
 * Initialize %Dataflash.
 */
int dataflash_setup(DataFlash *df, uintptr_t user_data);

/**
 * Initialise SPI interface for use with the %Dataflash,
 * allowing shared use with other SPI devices (which must however use
 * a different chip select pin).
 **/
void dataflash_begin(DataFlash *df);

/**
 * Restore SPI configuration, so it can be used by other SPI devices.
 **/
void dataflash_end(DataFlash *df);


/**
 * Set erase mode to automatic (default).
 **/
void dataflash_auto_erase(DataFlash *df);

/**
 * Set erase mode to manual.
 * User must erase pages first, using one of the erase commands.
 **/
void dataflash_manual_erase(DataFlash *df);

/**
 * Return whether the chip has completed the current operation and is
 * ready for the next.
 * Note that in some situations read/write access to one of the buffers
 * is permitted although the chip is busy.
 **/
bool dataflash_is_ready(DataFlash *df);

/**
 * @brief Wait until the chip is ready.
 * Perform a low-to-high transition on the CS pin and then poll
 * the status register until the %Dataflash is ready for the next
 * operation.
 */
void dataflash_wait_until_ready(DataFlash *df);
        
/**
 * Same as waitUntilReady
 **/
void dataflash_end_and_wait(DataFlash *df);
        
/**
 * Read status register.
 * @return The content of the status register.
 * **/
int dataflash_status(DataFlash *df);

/**
 * A main memory page read allows the user to read data directly from
 * any one of the pages in the main memory, bypassing both of the
 * data buffers and leaving the contents of the buffers unchanged.
 * Reading past the end of the page wraps around to the beginning of
 * the page.
 * The chip must remain enabled by this function; it is the user's
 * responsibility to disable the chip when finished reading.
 * @param df [todo]
 * @param page Page of the main memory to read.
 * @param offset Starting byte address within the page.
 * @return [todo]
 **/
int dataflash_page_read(DataFlash *df, uint16_t page, uint16_t offset);


/**
 * Sequentially read a continuous stream of data at the currently set
 * speed. Reading past the end of the last page wraps around to the
 * beginning of the first page.
 * The chip must remain enabled by this function; it is the user's
 * responsibility to disable the chip when finished reading.
 * @param df [todo]
 * @param page Page of the main memory where the sequential read will
 * start.
 * @param offset Starting byte address within the page.
 * @return [todo]
 * @note The legacy mode is not needed and not supported.
 **/
int dataflash_array_read(DataFlash *df, uint16_t page, uint16_t offset);

/**
 * Read the content of one of the SRAM data buffer at the currently
 * set speed. Reading past the end of the buffer wraps around to the
 * beginning.
 * The chip must remain enabled by this function; it is the user's
 * responsibility to disable the chip when finished reading.
 * @param df [todo]
 * @param buffer_num Buffer to read (0 or 1).
 * @param offset Starting byte within the buffer.
 * @return [todo]
 **/
int dataflash_buffer_read(DataFlash *df, uint8_t buffer_num, uint16_t offset);

/**
 * Write data to one of the SRAM data buffers at the currently set
 * speed. Writing past the end of the buffer wraps around to the
 * beginning.
 * The chip must remain enabled by this function; it is the user's
 * responsibility to disable the chip when finished reading.
 * @param df [todo]
 * @param buffer_num Buffer to read (0 or 1).
 * @param offset Starting byte within the buffer (default value: 0).
 * @return [todo]
 **/
int dataflash_buffer_write(DataFlash *df, uint8_t buffer_num, uint16_t offset);

/**
 * Transfer data from buffer 0 or 1 to a main memory page, erasing the
 * page first if auto-erase is set. If erase is manual, the page must
 * have been erased previously using one of the erase commands.
 * @param df [todo]
 * @param buffer_num Buffer to use (0 or 1).
 * @param page Page to which the content of the buffer is written.
 * @return [todo]
 **/
int dataflash_buffer_to_page(DataFlash *df, uint8_t buffer_num, uint16_t page);

/**
 * Transfer a page of data from main memory to buffer 0 or 1.
 * @param df [todo]
 * @param page Main memory page to transfer.
 * @param buffer_num Buffer (0 or 1) to which the data is written.
 * @return [todo]
 **/
int dataflash_page_to_buffer(DataFlash *df, uint16_t page, uint8_t buffer_num);

/**
 * Erase a page in the main memory array.
 * @param df [todo]
 * @param page Page to erase.
 * @return [todo]
 **/
int dataflash_page_erase(DataFlash *df, uint16_t page);

/**
 * Erase a block of pages in a single operation.
 * @param df [todo]
 * @param block Block to erase.
 * @return [todo]
 * @warning UNTESTED
 **/
int dataflash_block_erase(DataFlash *df, uint16_t block);

/**
 * Erase a sector of blocks in a single operation.
 * @param df [todo]
 * @param sector Sector to erase.
 * @return [todo]
 **/
int dataflash_sectore_erase(DataFlash *df, int8_t sector);

#ifdef AT45_CHIP_ERASE_ENABLED
/**
 * Erase the entire chip memory. Sectors protected or locked down will
 * not be erased.
 * @param df [todo]
 * @return [todo]
 * @warning UNTESTED
 * @warning MAY DAMAGE CHIP, THEREFORE NOT AVAILABLE.
 *          READ DATASHEET FOR DETAILS.
 **/
int dataflash_chip_erase(DataFlash *df);
#endif

/**
 * This a combination of Buffer Write and Buffer to Page with
 * Built-in Erase.
 * The global erase flag .manual_erase() is ignored.
 * Writing past the end of the page wraps around to the beginning of
 * the page.
 * @note You must call endAndWait in order to start transferring data
 * from buffer to page.
 * @param df [todo]
 * @param page Page to which the content of the buffer is written.
 * @param offset Starting byte address within the buffer.
 * @param buffer_num Buffer to use (0 or 1).
 * @return [todo]
 **/
int dataflash_begin_page_write_through_buffer(DataFlash *df, uint16_t page, uint16_t offset, uint8_t buffer_num);

/**
 * Compare a page of data in main memory to the data in buffer 0 or 1.
 * @param df [todo]
 * @param page Page to compare.
 * @param buffer_num Buffer number (0 or 1).
 * @return
 *      - true  If the page and the buffer contains the same data.
 *      - false Otherwise.
 **/
bool dataflash_is_page_equal_buffer(DataFlash *df, uint16_t page, uint8_t buffer_num);

/**
 * Put the device into the lowest power consumption mode.
 * Once the device has entered the Deep Power-down mode, all
 * instructions are ignored except the Resume from Deep
 * Power-down command.
 * @param df [todo]
 * @return [todo]
 * @warning UNTESTED
 **/
int dataflash_deep_power_down(DataFlash *df);

/**
 * Takes the device out of Deep Power-down mode.
 * @param df [todo]
 * @warning UNTESTED
 **/
int dataflash_resume_from_deep_power_down(DataFlash *df);

/**
 * Reset device via the reset pin.
 **/
void dataflash_hard_reset();

int dataflash_enable_sector_protection(DataFlash *df);
int dataflash_disable_sector_protection(DataFlash *df);
int dataflash_erase_sector_protection_register(DataFlash *df);

#if 0
        class SectorProtectionStatus
        {
          friend class DataFlash;
          public:
            SectorProtectionStatus();
            SectorProtectionStatus(const SectorProtectionStatus &status);
            SectorProtectionStatus& operator=(const SectorProtectionStatus& status);
            void set(int8_t sectorId, bool status);
            bool get(int8_t sectorId) const;
            void clear();
          private:
            uint8_t data[64];
        };
        
        uint8_t programSectorProtectionRegister(const SectorProtectionStatus& status);
        uint8_t readSectorProtectionRegister(SectorProtectionStatus& status);

/**
 * Same as waitUntilReady
 * @todo This method will be removed.
 **/
inline void DataFlash::endAndWait() {
    /* Wait for the end of the previous operation. */
    waitUntilReady();
}
#endif

/**
 * @}
 **/

#endif /* DATAFLASH_H */
