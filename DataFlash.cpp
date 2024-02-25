/**************************************************************************//**
 * @file DataFlash.cpp
 * @brief AT45DBxxxD Atmel Dataflash library for Arduino.
 *
 * @par Copyright: 
 * - Copyright (C) 2010-2011 by Vincent Cruz.
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
 *    - Released as at45db161d by Vincent Cruz.
 * - Version 2.0, 30 Aug 2011.
 *    - Based on the library by Vincent Cruz, dalek branch, of 25 Aug 2011.
 *      Substantially modified and improved by Volker Kuhlmann.
 *    - Allow a quick .begin() / .end() to switch the SPI interface between
 *      multiple SPI devices.
 *    - Efficiency improvements.
 *    - Re-arrange the mechanism to wait for the chip to become ready such that
 *      waiting only happens when necessary. This allows interleaved writing - fill
 *      up one buffer with new data while the other buffer is programmed to the 
 *      flash memory array. The downside is the the user now has to wait sometimes
 *      too, depending on the state of the flash chip and the user program.
 *    - Several improvements resulted in incompatible changes to the function API.
 *      This shouldn't matter because the %Dataflash library is in the process of
 *      evolving as an improvement of the at45db161d library and handles all the
 *      AT45DBxxxD flash ICs instead of just the AT45DB161D.
 * - Version 2.2, 29 Dec 2011.
 *    - Made to compile under Arduino 1.0 as well as 0.22.
 *    - Removed 4 compiler warnings.
 *    - Fixed a serious bug where accessing any pin set to -1 can cause random memory
 *      corruption (the Arduino pin functions like digitalWrite() have no range
 *      checking).
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

#include "DataFlash.h"
#include "DataFlashCommands.h"
 
/*
 * %Dataflash read/write addressing infos.
 * No need to put this into PROGMEM.
 */
static const struct {
    uint8_t buffer; /* Size of the buffer address bits. */
    uint8_t page;   /* Size of the page address bits. */
    uint8_t sector; /* Size of the sector address bits (part of the page address). */
} g_bit_size[7] = {
    {  9,  9, 2 }, //  1
    {  9, 10, 3 }, //  2
    {  9, 11, 3 }, //  4
    {  9, 12, 4 }, //  8
    { 10, 12, 4 }, // 16
    { 10, 13, 6 }, // 32
    { 11, 13, 5 }  // 64
};

/* Compute page address hi byte. */
static inline uint8_t page_to_hi_u8(uint16_t page, uint8_t buffer_size) {
    return page >> (16U - buffer_size);
}

/* Compute page address lo byte. */
static inline uint8_t page_to_lo_u8(uint16_t page, uint8_t buffer_size) {
    return page << (buffer_size  - 8U);
}

/* Disable (deselect) %Dataflash, then enable (select) it again.*/
static inline void dataflash_re_enable(DataFlash *df) {
    dataflash_disable(df);
    dataflash_enable(df);
}

int dataflash_read_id(DataFlash *df) {
    uint8_t buffer[5] = {
        [0] = DATAFLASH_READ_MANUFACTURER_AND_DEVICE_ID
    };
    int ret;

    // Reset command decoder.
    dataflash_re_enable(df);
    // Send manufacturer and device id read command.
    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U);

    /* Manufacturer ID */
    df->id.manufacturer = buffer[0];
    /* Device ID (part 1) */
    df->id.device[0] = buffer[1];
    /* Device ID (part 2) */
    df->id.device[1] = buffer[2];
    /* Extended Device Information String Length */
    df->id.extended_info_length = buffer[3];    

    dataflash_disable(df);
    return ret;
}

int dataflash_setup(DataFlash *df, uintptr_t user_data) {
    int status; 

    // [todo] validate function parameters
    df->user_data = user_data;
    df->erase = ERASE_AUTO;

#ifdef AT45_USE_SPI_SPEED_CONTROL
    df->speed = SPEED_LOW;
#endif
        
    /* Get DataFlash status register. */
    status = dataflash_status(df);
    if(status >= 0) {
        /* Bit 3 of status register is ignored as it's always 1. Note that it is
        * equal to 0 on the obsolete chip with density higher than 64 MB. */
        df->device_index = ((status & 0x38U) >> 3U) - 1U;
        /* If bit 0 is set, page size is set to 256 bytes, 264 otherwise. */
        df->bit_size.buffer = g_bit_size[df->device_index].buffer - (status & 1);
        df->bit_size.page   = g_bit_size[df->device_index].page;  
        df->bit_size.sector = g_bit_size[df->device_index].sector;
    }
    return status;
}

void dataflash_auto_erase(DataFlash *df) {
    df->erase = ERASE_AUTO;
}

void dataflash_manual_erase(DataFlash *df) {
    df->erase = ERASE_MANUAL;
}

bool dataflash_is_ready(DataFlash *df) {
    return (dataflash_status(df) & AT45_READY) == AT45_READY;
}

void dataflash_wait_until_ready(DataFlash *df) {
    /* Wait for the end of the transfer taking place. */
    while(!dataflash_is_ready(df)) {};
}

int dataflash_status(DataFlash *df) {
    uint8_t buffer[2] = {
        DATAFLASH_STATUS_REGISTER_READ,
        0x00
    };
    int ret;

    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout

    dataflash_disable(df);

    return (ret >= 0) ? (int)buffer[1] : ret;
}

int dataflash_page_read(DataFlash *df, uint16_t page, uint16_t offset) {
    uint8_t buffer[8] = {
        /* Opcode s*/
        DATAFLASH_PAGE_READ,
        /* Address (page | offset)  */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer) | (offset >> 8U),
        offset & 0xFFU,
        /* 4 "don't care" bytes */
        0x00, 0x00, 0x00, 0x00
    };

    /* Reset command decoder. */
    dataflash_re_enable(df);

    /* We do not disable chip here as we are just initiating read. */
    /* The user will have to read data from SPI and disable chip when done. */
    return dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout should be configurable
}

int dataflash_array_read(DataFlash *df, uint16_t page, uint16_t offset) {
    uint8_t buffer[] = {
        /* Opcode */
#ifdef AT45_USE_SPI_SPEED_CONTROL
        (df->speed == SPEED_LOW) ? DATAFLASH_CONTINUOUS_READ_LOW_FREQ : DATAFLASH_CONTINUOUS_READ_HIGH_FREQ,
#else
        DATAFLASH_CONTINUOUS_READ_LOW_FREQ,
#endif
        /* Address (page | offset)  */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer) | (offset >> 8U),
        offset & 0xFFU,
#ifdef AT45_USE_SPI_SPEED_CONTROL
        0x00
#endif
    };

    uint32_t len = sizeof(buffer);
#ifdef AT45_USE_SPI_SPEED_CONTROL
    /* High frequency continuous read has an additional don't care byte. */
    if(df->speed != SPEED_HIGH) {
        len--;
    }
#endif

    /* Reset command decoder. */
    dataflash_re_enable(df);
    return dataflash_transfer(df, buffer, len, 100U); // [todo] timeout should be configurable
}

int dataflash_buffer_read(DataFlash *df, uint8_t buffer_num, uint16_t offset) {
    uint8_t buffer[] = {
        /* Opcode */
#ifdef AT45_USE_SPI_SPEED_CONTROL
        (df->speed == SPEED_LOW) 
            ? (buffer_num ? DATAFLASH_BUFFER_2_READ_LOW_FREQ : DATAFLASH_BUFFER_1_READ_LOW_FREQ)
            : (buffer_num ? DATAFLASH_BUFFER_2_READ : DATAFLASH_BUFFER_1_READ)
#else
        (buffer_num ? DATAFLASH_BUFFER_2_READ_LOW_FREQ : DATAFLASH_BUFFER_1_READ_LOW_FREQ),
#endif
        /* 14 "Don't care" bits */
        0x00,
        /* Rest of the "don't care" bits + bits 8,9 of the offset */
        offset >> 8,
        /* bits 7-0 of the offset */
        offset & 0xFFU,    
#ifdef AT45_USE_SPI_SPEED_CONTROL
        0x00
#endif
    };

    uint32_t len = sizeof(buffer);
#ifdef AT45_USE_SPI_SPEED_CONTROL
    /* High frequency continuous read has an additional don't care byte. */
    if(df->speed != SPEED_HIGH) {
        len--;
    }
#endif

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    return dataflash_transfer(df, buffer, len, 100U); // [todo] timeout should be configurable
}

int dataflash_buffer_write(DataFlash *df, uint8_t buffer_num, uint16_t offset) {
    uint8_t buffer[] = {
        /* Opcode */
        buffer_num ? DATAFLASH_BUFFER_2_WRITE : DATAFLASH_BUFFER_1_WRITE,
        /* 14 "Don't care" bits */
        0x00,
        /* Rest of the "don't care" bits + bits 8,9 of the offset */
        offset >> 8,
        /* bits 7-0 of the offset */
        offset & 0xFFU
    };

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    return dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout should be configurable
}

int dataflash_buffer_to_page(DataFlash *df, uint8_t buffer_num, uint16_t page) {
    uint8_t buffer[4] = {
        /* Opcode */
        (df->erase == ERASE_AUTO) 
            ? (buffer_num ? DATAFLASH_BUFFER_2_TO_PAGE_WITH_ERASE
                          : DATAFLASH_BUFFER_1_TO_PAGE_WITH_ERASE)
            : (buffer_num ? DATAFLASH_BUFFER_2_TO_PAGE_WITHOUT_ERASE
                          : DATAFLASH_BUFFER_1_TO_PAGE_WITHOUT_ERASE),
        /* see dataflash_page_to_buffer */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer),
        0x00        
    };
    int ret;

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U);        // [todo]

    /* Start transfer. If erase was set to automatic, the page will first be
    erased. The chip remains busy until this operation finishes. */
    dataflash_disable(df);

    return ret;
}

int dataflash_page_to_buffer(DataFlash *df, uint16_t page, uint8_t buffer_num) {
    uint8_t buffer[4] = {
        /* Opcode */
        buffer_num ? DATAFLASH_TRANSFER_PAGE_TO_BUFFER_2
                   : DATAFLASH_TRANSFER_PAGE_TO_BUFFER_1,
        /* 3 bytes adress.
        * For all DataFlashes 011D to 642D the number of trailing don't care bits
        * is equal to the number of page bits plus 3 (a block consists of 8 (1<<3)
        * pages), and always larger than 8 so the third byte is always 0. */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer),
        0x00
    };
    int ret;

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout
    
    /* Start transfer. The chip remains busy until this operation finishes. */
    dataflash_disable(df);

    return ret;
}

int dataflash_page_erage(DataFlash *df, uint16_t page) {
    uint8_t buffer[4] = {
        /* Opcode */
        DATAFLASH_PAGE_ERASE,
        /* see pageToBuffer */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer),
        0x00
    };

    int ret;

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout
    
    /* Start page erase. The chip remains busy until this operation finishes. */
    dataflash_disable(df);

    return ret;
}

int dataflash_block_erase(DataFlash *df, uint16_t block) {
    /* 3 bytes adress.
     * For all DataFlashes 011D to 642D the number of trailing don't care bits
     * is equal to the number of page bits plus 3 (a block consists of 8 (1<<3)
     * pages), and always larger than 8 so the third byte is always 0. */
    uint8_t right_shift = df->bit_size.buffer + 3 - 8;
    uint16_t addr = block >> right_shift;

    uint8_t buffer[4] = {
        /* Opcode */
        DATAFLASH_BLOCK_ERASE,
        /* Address */
        addr >> 8,
        addr & 0xFF,
        0x00
    };

    int ret;

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout

    /* Start block erase.
    The chip remains busy until this operation finishes. */
    dataflash_disable(df);

    return ret;
}

int dataflash_sectore_erase(DataFlash *df, int8_t sector) {
    uint8_t buffer[4] = {
        /* Opcode */
        [0] = DATAFLASH_SECTOR_ERASE,
        [3] = 0x00
    };
	
    if((sector == AT45_SECTOR_0A) || (sector == AT45_SECTOR_0B)) {
        buffer[1] = 0x00;
        buffer[2] = ((uint8_t)(-sector) & 0x01) << (df->bit_size.buffer - 5U);
    } else {
        uint8_t shift = df->bit_size.buffer + df->bit_size.page - df->bit_size.sector - 16U;        
        buffer[1] = sector << shift;
        buffer[2] = 0x00;
    }

    int ret;

    /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);
    
    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout

    /* Start sector erase.
    The chip remains busy until this operation finishes. */
    dataflash_disable(df);

    return ret;
}

#ifdef AT45_CHIP_ERASE_ENABLED
/** 
 * Erase the entire chip memory. Sectors protected or locked down will
 * not be erased.
 * @warning UNTESTED
 **/
int dataflash_chip_erase(DataFlash *df) {
    uint8_t sector_count = 1 << df->sectore.size;
    int ret;

    ret = dataflash_sector_erase(df, AT45_SECTOR_0A);
    if(ret >= 0) {
        ret = dataflash_sector_erase(df, AT45_SECTOR_0B);
        for(uint8_t i=1; (ret>=0) && (i<sector_count); i++) {
            ret = dataflash_sector_erase(df, i);
        }
    }    
#if 0
    /* DO NOT USE THIS CODE!       */
    /* MAY DAMAGE CHIP.            */
    /* READ DATASHEET FOR DETAILS. */
 
    uint8_t buffer[4] = {
        /* Chip erase sequence */
        DATAFLASH_CHIP_ERASE_0,
        DATAFLASH_CHIP_ERASE_1,
        DATAFLASH_CHIP_ERASE_2,
        DATAFLASH_CHIP_ERASE_3
    };

   /* Wait for the end of the previous operation. */
    dataflash_wait_until_ready(df);
    /* Reset command decoder. */
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout
                
    /* Start chip erase.
    The chip remains busy until this operation finishes. */
    dataflash_disable(df);

#else
    (void) sector;
#endif
}
#endif // AT45_CHIP_ERASE_ENABLED

int dataflash_begin_page_write_through_buffer(DataFlash *df, uint16_t page, uint16_t offset, uint8_t buffer_num) {
    uint8_t buffer[4] = {
        /* Opcode */
        buffer_num ? DATAFLASH_PAGE_THROUGH_BUFFER_2
                   : DATAFLASH_PAGE_THROUGH_BUFFER_1,
        /* Address */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer) | (offset>>8),
        offset & 0xFFU
    };
    
    /* Reset command decoder. */
    dataflash_re_enable(df);

    return dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout
}

bool dataflash_is_page_equal_buffer(DataFlash *df, uint16_t page, uint8_t buffer_num) {
    uint8_t buffer[4] = {
        /* Opcode */
        buffer_num ? DATAFLASH_COMPARE_PAGE_TO_BUFFER_2
                   : DATAFLASH_COMPARE_PAGE_TO_BUFFER_1,
        /* Page address */
        page_to_hi_u8(page, df->bit_size.buffer),
        page_to_lo_u8(page, df->bit_size.buffer),
        0x00
    };

    int status;
    bool ret = false;

    /* Reset command decoder. */
    dataflash_re_enable(df);
   
    status = dataflash_transfer(df, buffer, sizeof(buffer), 100U); // [todo] timeout

    /* Start comparison */
    dataflash_disable(df);

    /* Wait for the end of the comparison. */
    dataflash_wait_until_ready(df);
        
    /* If bit 6 of the status register is 0 then the data in the
     * main memory page matches the data in the buffer. 
     * If it's 1 then the data in the main memory page doesn't match.
     */
    if(status >= 0) {
        status = dataflash_status(df);
        if(status >= 0) {
            ret = (status & AT45_COMPARE) == 0;
        }
    }
    return ret;
}

int dataflash_deep_power_down(DataFlash *df) {
    uint8_t cmd = DATAFLASH_DEEP_POWER_DOWN;
    int ret;
    /* Reset command decoder. */
    dataflash_re_enable(df);

    /* Send opcode */
    ret = dataflash_transfer(df, &cmd, 1, 100U); // [todo] timeout

    /* Enter Deep Power-Down mode */
    dataflash_disable(df);

    return ret;
}

int dataflash_resume_from_deep_power_down(DataFlash *df) {
    uint8_t cmd = DATAFLASH_RESUME_FROM_DEEP_POWER_DOWN;
    int ret;

    /* Reset command decoder. */
    dataflash_re_enable(df);
    
     /* Send opcode */
    ret = dataflash_transfer(df, &cmd, 1, 100U); // [todo] timeout
    
    /* Enter Deep Power-Down mode */
    dataflash_disable(df);
    
    /* The CS pin must stay high during t_RDPD microseconds before the device
     * can receive any commands.
     * On the at45db161D t_RDPD = 35 microseconds.
     * Wait 40us (just to be sure). */
    dataflash_usleep(40U);
    return ret;
}

void dataflash_hard_reset(DataFlash *df) {
    if (dataflash_toggle_reset(df, false)) {
        /* The reset pin should stay low for at least 10us (table 18.4). */
        dataflash_usleep(10U);
        
        /* According to the Dataflash spec (21.6 Reset Timing),
         * the CS pin should be in high state before RESET
         * is deasserted (ie HIGH). */
        dataflash_disable(df);
        /* Just to be sure that the high state is reached */
        dataflash_usleep(1U);
            
        dataflash_toggle_reset(df, true);
        
        /* Reset recovery time = 1us */
        dataflash_usleep(1U);
    }
}

int dataflash_enable_sector_protection(DataFlash *df) {
    uint8_t buffer[4] = {
        DATAFLASH_ENABLE_SECTOR_PROTECTION_0,
        DATAFLASH_ENABLE_SECTOR_PROTECTION_1,
        DATAFLASH_ENABLE_SECTOR_PROTECTION_2,
        DATAFLASH_ENABLE_SECTOR_PROTECTION_3
    };
    int ret;

    dataflash_wait_until_ready(df);
    dataflash_write_protect(df, true);
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U);     // [todo] timeout

    dataflash_disable(df);
    dataflash_write_protect(df, false);

    return ret;
}
  
int dataflash_disable_sector_protection(DataFlash *df) {
    uint8_t buffer[4] = {
        DATAFLASH_DISABLE_SECTOR_PROTECTION_0,
        DATAFLASH_DISABLE_SECTOR_PROTECTION_1,
        DATAFLASH_DISABLE_SECTOR_PROTECTION_2,
        DATAFLASH_DISABLE_SECTOR_PROTECTION_3
    };
    int ret;

    dataflash_wait_until_ready(df);
    dataflash_write_protect(df, true);
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U);     // [todo] timeout

    dataflash_disable(df);

    return ret;
}

int dataflash_erase_sector_protection_register(DataFlash *df) {
    uint8_t buffer[4] = {
        DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_0,
        DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_1,
        DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_2,
        DATAFLASH_ERASE_SECTOR_PROTECTION_REGISTER_3
    };
    int ret;

    dataflash_wait_until_ready(df);
    dataflash_write_protect(df, true);
    dataflash_re_enable(df);

    ret = dataflash_transfer(df, buffer, sizeof(buffer), 100U);     // [todo] timeout

    dataflash_disable(df);

    dataflash_wait_until_ready(df);

    dataflash_write_protect(df, false);

    return ret;
}
#if 0
uint8_t DataFlash::programSectorProtectionRegister(const DataFlash::SectorProtectionStatus& status)
{
    uint8_t sectorCount = 1 << m_sectorSize;
    eraseSectorProtectionRegister();

    if(m_writeProtectPin >= 0)
        digitalWrite(m_writeProtectPin, HIGH);
    reEnable();

    SPI.transfer(DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_0);
    SPI.transfer(DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_1);
    SPI.transfer(DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_2);
    SPI.transfer(DATAFLASH_PROGRAM_SECTOR_PROTECTION_REGISTER_3);

    for(uint8_t i=0; i<sectorCount; i++)
    {
        SPI.transfer(status.data[i]);
    }

    disable();
    waitUntilReady();
    if(m_writeProtectPin >= 0)
        digitalWrite(m_writeProtectPin, LOW);

    return sectorCount;
}

uint8_t DataFlash::readSectorProtectionRegister(DataFlash::SectorProtectionStatus& status)
{
    uint8_t sectorCount = 1 << m_sectorSize;

    waitUntilReady();
    reEnable();

    SPI.transfer(DATAFLASH_READ_SECTOR_PROTECTION_REGISTER);
    SPI.transfer(0xff);
    SPI.transfer(0xff);
    SPI.transfer(0xff);

    for(uint8_t i=0; i<sectorCount; i++)
    {
        status.data[i] = SPI.transfer(0);
    }

    disable();

    return sectorCount;
}

DataFlash::SectorProtectionStatus::SectorProtectionStatus()
{
    clear();
}
DataFlash::SectorProtectionStatus::SectorProtectionStatus(const DataFlash::SectorProtectionStatus &status)
{
  for(uint8_t i=0; i<64; i++)
  {
    data[i] = status.data[i];
  }
}
DataFlash::SectorProtectionStatus& DataFlash::SectorProtectionStatus::operator=(const DataFlash::SectorProtectionStatus& status)
{
  for(uint8_t i=0; i<64; i++)
  {
    data[i] = status.data[i];
  }
  return *this;
}
void DataFlash::SectorProtectionStatus::set(int8_t sectorId, bool status)
{
    if(sectorId == AT45_SECTOR_0A)
    {
        data[0] = (data[0] & 0x3f) | (status ? 0xc0 : 0x00);
    }
    else if(sectorId == AT45_SECTOR_0B)
    {
        data[0] = (data[0] & 0xcf) | (status ? 0x30 : 0x00);
    }
    else if((sectorId > 0) && (sectorId < 64))
    {
        data[sectorId] = status ? 0xff : 0x00;
    }
}
bool DataFlash::SectorProtectionStatus::get(int8_t sectorId) const
{
    if(sectorId == AT45_SECTOR_0A)
    {
        return (data[0] & 0xc0) ? true : false;
    }
    else if(sectorId == AT45_SECTOR_0B)
    {
        return (data[0] & 0x30) ? true : false;
    }
    else if((sectorId > 0) && (sectorId < 64))
    {
        return data[sectorId] ? true : false;
    }
    return false;
}
void DataFlash::SectorProtectionStatus::clear()
{
    for(uint8_t i=0; i<64; i++)
    {
      data[i] = 0;
    }
}
#endif
/**
 * @}
 **/
