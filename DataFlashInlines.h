/*
********************************************************************************
 DataFlashInlines.h
 
 File containing definitions for inline methods and functions for the
 Atmel DataFlash Arduino library.
 
 Copyright (C) 2011 by Vincent Cruz.
 cruz.vincent@gmail.com 
 
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
 
 
1.0  04 Sep 2011   Created.
********************************************************************************
*/

/**
 * Enable device with chipselect.
 **/
inline void DataFlash::enable()
{
    digitalWrite(m_chipSelectPin, LOW);
}

/**
 * Disable device with chipselect.
 **/
inline void DataFlash::disable()
{
    digitalWrite(m_chipSelectPin, HIGH);
}

/**
 * Enable write protection.
 **/
inline void DataFlash::writeProtect()
{
    digitalWrite(m_writeProtectPin, LOW);
}

/**
 * Disable write protection.
 **/
inline void DataFlash::readWrite()
{
    digitalWrite(m_writeProtectPin, HIGH);
}

/** Get chip Select (CS) pin **/
inline uint8_t DataFlash::chipSelectPin  () const
{
    return m_chipSelectPin;
}

/** Get reset (RESET) pin **/
inline uint8_t DataFlash::resetPin       () const
{
    return m_resetPin;
}

/** Get write protect (WP) pin **/
inline uint8_t DataFlash::writeProtectPin() const
{
    return m_writeProtectPin;
}

/**
 * Compute page address high byte.
 */
inline uint8_t DataFlash::pageToHiU8(uint16_t page) const
{
    return page >> (16 - m_bufferSize);
}

/**
 * Compute page address low byte.
 */
inline uint8_t DataFlash::pageToLoU8(uint16_t page) const
{
    return page << (m_bufferSize  - 8);
}

/**
 * Same as @see waitUntilReady
 * @todo This method will be removed.
 **/
inline void DataFlash::endAndWait()
{
    /* Wait for the end of the previous operation. */
    waitUntilReady();
}
