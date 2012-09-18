/**************************************************************************//**
 * @file Infos.h
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
#ifndef INFOS_H
#define INFOS_H

/**
 * @addtogroup DummyUnitTest
 * @{
 **/
namespace Dummy
{
    #define MAX_VALUE_STRING_SIZE 16

    /** General informations. **/
    class Infos
    {
      public:
        /**
		 * Constructor.
         * @param [in] filename : Filename.
         * @param [in] suiteName : Test suit name.
         * @param [in] testName : Test name.
         * @param [in] line : Line number.
		 **/
        Infos(char const* filename, char const* suiteName, char const* testName, int line);
        
		inline char const* Filename () const { return m_filename;  } /**< Get filename.   **/
		inline char const* SuiteName() const { return m_suiteName; } /**< Get suite name. **/
		inline char const* TestName () const { return m_testName;  } /**< Get test name.  **/
		inline int         Line     () const { return m_line;      } /**< Get line.       **/
		
        /** Destructor. **/
		~Infos() {}

      private:
		char const* m_filename;  /**< Filename.        **/
		char const* m_suiteName; /**< Test suite name. **/
		char const* m_testName;  /**< Test name.       **/
		int         m_line;      /**< Line number.     **/
    };
};
/** @} **/

#endif // INFOS_H