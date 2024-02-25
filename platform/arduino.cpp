/**************************************************************************//**
 * @file DataFlash.h
 * @brief AT45DBxxxD Atmel Dataflash library for Arduino.
 *
 * @par Copyright: 
 * - Copyright (C) 2010-2024 by Vincent Cruz.
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
 * - Version 3.0, Mar 2024.
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
#include "arduino.h"

extern "C" void dataflash_usleep(uint64_t t) {
    delayMicroseconds(t);
}

extern "C" void dataflash_enable(DataFlash *df) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    digitalWrite(impl->cs, LOW);
}

extern "C" void dataflash_disable(DataFlash *df) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    digitalWrite(impl->cs, HIGH);
}

extern "C" void dataflash_begin(DataFlash *df) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    SPI.beginTransaction(impl->settings);
    dataflash_enable(df);
}

extern "C" void dataflash_end(DataFlash *df) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    dataflash_disable(df);
    SPI.endTransaction();
}

extern "C" int dataflash_transfer(DataFlash *df, uint8_t *buffer, uint32_t size, uint32_t timeout) {
    (void)df;
    (void)timeout;
    SPI.transfer(buffer, size);
}

extern "C" bool dataflash_toggle_reset(DataFlash *df, bool state) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    digitalWrite(impl->reset, state ? LOW : HIGH);
}

extern "C" bool dataflash_write_protect(DataFlash *df, bool state) {
    DataFlashArduino *impl = reinterpret_cast<DataFlashArduino*>(df);
    digitalWrite(impl->wp, state ? LOW : HIGH);
}

// [todo]
int dataflash_arduino_setup(DataFlash *df, int8_t cs, int8_t reset, int8_t wp) {
    DataFlashArduino *impl = (DataFlashArduino*)df;
    impl->cs    = cs;
    impl->reset = reset;
    impl->wp    = wp;

    pinMode(impl->cs, OUTPUT);
    if(impl->reset >= 0) {
        pinMode(impl->reset, OUTPUT);
        digitalWrite(impl->reset, HIGH); // set inactive
    }
    if(impl->wp >= 0) {
        pinMode(impl->wp, OUTPUT);
        digitalWrite(impl->wp, HIGH); // set inactive
    }

    /* Setup SPI */
    impl->settings = SPISettings(4000000, MSBFIRST, SPI_MODE3);
    dataflash_begin(df);
    int ret = dataflash_setup(df);
    dataflash_end(df);

    return ret;
}