/**************************************************************************//**
 * @file Utils.h
 *
 * @par Copyright: 
 * - Copyright (C) 2011 by Vincent Cruz.
 * All rights reserved.
 *
 * @author Vincent Cruz cruz.vincent@gmail.com
 *
 * @par Description:
 *  Simple Unit test framework. @n
 *  Based on UnitTest++ by Noel Llopis and Charles Nicholson. @n
 *  This is basically a stripped down version meant to be run on a
 *  microcontroller.
 *
 * @par History:
 * - Version 0.1, 28 Dec 2011.
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
#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>

namespace Dummy
{
    /**
     * @addtogroup DummyUnitTest
     * @{
     **/

    /**
     * @defgroup IsEqual Values test.
     * @{
     **/
	template <typename T>
	bool IsEqual(T const& expected, T const& value)
	{
		return (expected == value);
	}
	bool IsEqual(char const* expected, char const* value);
	bool IsEqual(char* expected, char const* value);
    bool IsEqual(char* expected, char* value);
    /** @} **/
    
    /**
     * @defgroup Stringify String conversion functions.
     * @{
     **/
    int Stringify(char value, char* buffer, int bufferLen);
    int Stringify(int value, char* buffer, int bufferLen);
    int Stringify(float value, char* buffer, int bufferLen);
    int Stringify(char const* value, char* buffer, int bufferLen);
    int Stringify(char* value, char* buffer, int bufferLen);
    int Stringify(void const* value, char* buffer, int bufferLen);
    /** @} **/
    /** @} **/
}

#endif // UTILS_H