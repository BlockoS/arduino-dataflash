/**************************************************************************//**
 * @file Result.h
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
#ifndef RESULT_H
#define RESULT_H

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
    /** Test cases result. **/
    struct Result
    {
        /** Reset counters. **/
        inline void Reset();
        
        int total;  /**< Number of tests run. **/
        int failed; /**< Number of failed tests. **/
        int error;  /**< Number of errors (CHECK failed). **/
    };
    
    /** Reset counters. **/
    inline void Result::Reset()
    {
        total = 0, failed = 0, error = 0;
    }
        
};
/** @} **/

#endif // RESULT_H