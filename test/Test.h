/*
Simple Unit test framework.
Based on UnitTest++ by Noel Llopis and Charles Nicholson.
This is basically a stripped down version meant to be run on a
microcontroller.

History:

Version 0.1, 2011

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
*/
#ifndef TEST_H
#define TEST_H

#include "Runner.h"

/**
 * @defgroup DummyUnitTest Simple unit test framework.
 * @{
 **/
namespace Dummy
{
    /** Test interface. **/
    class Test
    {
      friend class Runner;
      public:
        /**
		 * Constructor.
         * @param [in] name : Test name.
		 */
        explicit Test(const char* name);
        
        /** Destructor. **/
        virtual ~Test();  
        
        /** Run test. **/
        virtual void Run() = 0;
        
      protected:
        bool m_success;     /**< Evaluation result. **/
        const char* m_name; /**< Name. **/
        
        Test *m_previous;   /**< Previous test. **/
        Test *m_next;       /**< Next test to be run. **/
    };
};
/** @} **/

#endif // TEST_H