
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
#ifndef DATAFLASH_ARDUINO_H
#define DATAFLASH_ARDUINO_H

#if ARDUINO >= 100
#   include <Arduino.h>
#else
#   include <WProgram.h>
#endif

#include <SPI.h>

#include "DataFlash.h"

// [todo]
typedef struct  {
    DataFlash base;
    int8_t cs;              /**< Chip select pin (CS). **/
    int8_t reset;           /**< Reset pin (RESET). **/
    int8_t wp;              /**< Write protect pin (WP). **/
    SPISettings settings;   /**< SPI port configuration **/
} DataFlashArduino;

// [todo]
int dataflash_arduino_setup(DataFlash *df, int8_t cs, int8_t reset, int8_t wp);

#endif // DATAFLASH_ARDUINO_H
